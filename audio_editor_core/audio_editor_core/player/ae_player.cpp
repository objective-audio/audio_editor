//
//  ae_player.cpp
//

#include "ae_player.h"

#include <audio/yas_audio_umbrella.h>
#include <audio_editor_core/ae_scrolling_types.h>
#include <playing/yas_playing_umbrella.h>

using namespace yas;
using namespace yas::ae;

player::player(std::shared_ptr<playing::coordinator> const &coordinator, std::string const &identifier,
               std::shared_ptr<scrolling_for_player> const &scrolling)
    : _identifier(identifier), _coordinator(coordinator), _scrolling(scrolling) {
    scrolling
        ->observe([this](scrolling_event const &event) {
            switch (event.state) {
                case scrolling_state::began:
                    this->_reserved_frame = this->_coordinator->current_frame();
                    this->_seeking_frame = std::nullopt;
                    this->_update_reserved_frame_if_began(event.delta_time);
                    break;
                case scrolling_state::changed:
                    this->_update_reserved_frame_if_began(event.delta_time);
                    break;
                case scrolling_state::ended:
                    this->_update_reserved_frame_if_began(event.delta_time);
                    if (this->_reserved_frame.has_value()) {
                        this->seek(this->_reserved_frame.value());
                        this->_reserved_frame = std::nullopt;
                    }
                    break;
            }
        })
        .end()
        ->add_to(this->_pool);
}

void player::begin_rendering() {
    this->_coordinator->set_rendering(true);
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
    if (auto const scrolling = this->_scrolling.lock()) {
        scrolling->set_is_enabled(!is_playing);
    }

    this->_coordinator->set_playing(is_playing);
}

bool player::is_playing() const {
    return this->_coordinator->is_playing();
}

void player::seek(frame_index_t const frame) {
    if (this->_coordinator->current_frame() != frame) {
        this->_coordinator->seek(frame);
        this->_seeking_frame = frame;
    }
}

bool player::is_scrolling() const {
    if (auto const scrolling = this->_scrolling.lock()) {
        return scrolling->is_began();
    } else {
        return false;
    }
}

frame_index_t player::current_frame() const {
    if (this->_reserved_frame.has_value()) {
        return this->_reserved_frame.value();
    } else if (this->_coordinator->is_seeking() && this->_seeking_frame.has_value()) {
        return this->_seeking_frame.value();
    } else {
        return this->_coordinator->current_frame();
    }
}

observing::syncable player::observe_is_playing(std::function<void(bool const &)> &&handler) {
    return this->_coordinator->observe_is_playing(std::move(handler));
}

void player::_update_reserved_frame_if_began(double const delta_time) {
    if (this->_reserved_frame.has_value() && (delta_time != 0)) {
        auto const &sample_rate = this->_coordinator->format().sample_rate;
        auto const reserved_time = static_cast<double>(this->_reserved_frame.value()) / sample_rate;
        auto const reserving_time = reserved_time + delta_time;
        this->_reserved_frame = static_cast<int64_t>(reserving_time * sample_rate);
    }
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
