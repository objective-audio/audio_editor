//
//  ae_player.cpp
//

#include "ae_player.h"

#include <audio/yas_audio_umbrella.h>
#include <playing/yas_playing_umbrella.h>

using namespace yas;
using namespace yas::ae;

player::player(std::shared_ptr<playing::coordinator> const &coordinator, std::string const &identifier,
               std::shared_ptr<scrolling_for_player> const &scrolling)
    : _identifier(identifier), _coordinator(coordinator), _scrolling(scrolling) {
}

void player::set_timeline(proc::timeline_ptr const &time_line, playing::sample_rate_t const sample_rate,
                          audio::pcm_format const pcm_format) {
    this->_coordinator->set_timeline(time_line, this->_identifier);
    this->_coordinator->set_timeline_format(sample_rate, pcm_format);
}

void player::reset_timeline() {
    this->_coordinator->reset_timeline();
}

void player::set_playing(bool const is_playing) {
    this->_coordinator->set_playing(is_playing);
}

bool player::is_playing() const {
    return this->_coordinator->is_playing();
}

void player::seek(frame_index_t const frame) {
    this->_coordinator->seek(frame);
}

frame_index_t player::current_frame() const {
    return this->_coordinator->current_frame();
}

observing::syncable player::observe_is_playing(std::function<void(bool const &)> &&handler) {
    return this->_coordinator->observe_is_playing(std::move(handler));
}

std::shared_ptr<player> player::make_shared(url const &root_url, std::string const &identifier,
                                            std::shared_ptr<scrolling_for_player> const &scrolling) {
    return make_shared(
        playing::coordinator::make_shared(
            root_url.path(), playing::renderer::make_shared(audio::mac_device::renewable_default_output_device())),
        identifier, scrolling);
}

std::shared_ptr<player> player::make_shared(std::shared_ptr<playing::coordinator> const &coordinator,
                                            std::string const &identifier,
                                            std::shared_ptr<scrolling_for_player> const &scrolling) {
    return std::shared_ptr<player>(new player{coordinator, identifier, scrolling});
}
