/*
 *  Copyright (C) 2017-2022 Igalia S.L. All rights reserved.
 *  Copyright (C) 2022 Metrological Group B.V.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 */

#pragma once

#if USE(GSTREAMER_WEBRTC)

#include "GUniquePtrGStreamer.h"
#include "RealtimeIncomingSourceGStreamer.h"

namespace WebCore {

class RealtimeIncomingVideoSourceGStreamer : public RealtimeIncomingSourceGStreamer {
public:
    static Ref<RealtimeIncomingVideoSourceGStreamer> create(AtomString&& videoTrackId) { return adoptRef(*new RealtimeIncomingVideoSourceGStreamer(WTFMove(videoTrackId))); }
    ~RealtimeIncomingVideoSourceGStreamer() = default;

    // RealtimeMediaSource API
    bool isIncomingVideoSource() const final { return true; }

    const GstStructure* stats();

    void configureForInputCaps(const GRefPtr<GstCaps>&) final;

protected:
    RealtimeIncomingVideoSourceGStreamer(AtomString&&);

private:
    // RealtimeMediaSource API
    void settingsDidChange(OptionSet<RealtimeMediaSourceSettings::Flag>) final;
    const RealtimeMediaSourceSettings& settings() final;

    // RealtimeIncomingSourceGStreamer API
    void dispatchSample(GRefPtr<GstSample>&&) final;

    void ensureSizeAndFramerate(const GRefPtr<GstCaps>&);

    std::optional<RealtimeMediaSourceSettings> m_currentSettings;
    GUniquePtr<GstStructure> m_stats;

    bool m_isDecoding { false };
    FloatSize m_videoSize;
    uint64_t m_decodedVideoFrames { 0 };
    GRefPtr<GstElement> m_queue;
    GRefPtr<GstElement> m_fakeVideoSink;
};

} // namespace WebCore

SPECIALIZE_TYPE_TRAITS_BEGIN(WebCore::RealtimeIncomingVideoSourceGStreamer)
    static bool isType(const WebCore::RealtimeMediaSource& source) { return source.isIncomingVideoSource(); }
SPECIALIZE_TYPE_TRAITS_END()

#endif // USE(GSTREAMER_WEBRTC)
