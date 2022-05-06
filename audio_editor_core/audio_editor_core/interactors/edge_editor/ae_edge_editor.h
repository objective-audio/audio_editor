//
//  ae_edge_editor.h
//

#pragma once

#include <memory>

namespace yas::ae {
class edge_holder;
class player;
class exporter;

struct edge_editor final {
    [[nodiscard]] static std::shared_ptr<edge_editor> make_shared(std::string const &project_id,
                                                                  std::shared_ptr<edge_holder> const &,
                                                                  std::shared_ptr<exporter> const &);

    [[nodiscard]] bool can_set_begin() const;
    [[nodiscard]] bool can_set_end() const;
    void set_begin();
    void set_end();

   private:
    std::weak_ptr<edge_holder> const _holder;
    std::weak_ptr<player> const _player;
    std::weak_ptr<exporter> const _exporter;

    edge_editor(std::shared_ptr<edge_holder> const &, std::shared_ptr<player> const &,
                std::shared_ptr<exporter> const &);

    bool _can_editing() const;
};
}  // namespace yas::ae
