/*
 * Copyright (c) 2021-2022 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#import "BindableResource.h"
#import <wtf/FastMalloc.h>
#import <wtf/Ref.h>
#import <wtf/RefCounted.h>
#import <wtf/Vector.h>

struct WGPUBindGroupImpl {
};

namespace WebGPU {

class Device;

// https://gpuweb.github.io/gpuweb/#gpubindgroup
class BindGroup : public WGPUBindGroupImpl, public RefCounted<BindGroup> {
    WTF_MAKE_FAST_ALLOCATED;
public:
    static constexpr MTLRenderStages MTLRenderStageCompute = static_cast<MTLRenderStages>(0);
    static constexpr MTLRenderStages MTLRenderStageUndefined = static_cast<MTLRenderStages>(MTLRenderStageFragment + 1);
    static Ref<BindGroup> create(id<MTLBuffer> vertexArgumentBuffer, id<MTLBuffer> fragmentArgumentBuffer, id<MTLBuffer> computeArgumentBuffer, Vector<BindableResources>&& resources, Device& device)
    {
        return adoptRef(*new BindGroup(vertexArgumentBuffer, fragmentArgumentBuffer, computeArgumentBuffer, WTFMove(resources), device));
    }
    static Ref<BindGroup> createInvalid(Device& device)
    {
        return adoptRef(*new BindGroup(device));
    }

    ~BindGroup();

    void setLabel(String&&);

    bool isValid() const { return m_vertexArgumentBuffer || m_fragmentArgumentBuffer || m_computeArgumentBuffer; }

    id<MTLBuffer> vertexArgumentBuffer() const { return m_vertexArgumentBuffer; }
    id<MTLBuffer> fragmentArgumentBuffer() const { return m_fragmentArgumentBuffer; }
    id<MTLBuffer> computeArgumentBuffer() const { return m_computeArgumentBuffer; }

    const Vector<BindableResources>& resources() const { return m_resources; }

    Device& device() const { return m_device; }
    static bool allowedUsage(const OptionSet<BindGroupEntryUsage>&);
    static uint64_t makeEntryMapKey(uint32_t baseMipLevel, uint32_t baseArrayLayer, WGPUTextureAspect);

private:
    BindGroup(id<MTLBuffer> vertexArgumentBuffer, id<MTLBuffer> fragmentArgumentBuffer, id<MTLBuffer> computeArgumentBuffer, Vector<BindableResources>&&, Device&);
    BindGroup(Device&);

    const id<MTLBuffer> m_vertexArgumentBuffer { nil };
    const id<MTLBuffer> m_fragmentArgumentBuffer { nil };
    const id<MTLBuffer> m_computeArgumentBuffer { nil };

    const Ref<Device> m_device;
    Vector<BindableResources> m_resources;
};

} // namespace WebGPU
