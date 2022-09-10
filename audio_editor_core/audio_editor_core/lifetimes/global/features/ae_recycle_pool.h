//
//  ae_recycle_pool.h
//

#pragma once

#include <audio_editor_core/ae_common_types.h>
#include <cpp_utils/yas_identifier.h>
#include <observing/yas_observing_umbrella.h>

namespace yas::ae {
enum class recycle_pool_event_type {
    fetched,
    replaced,
    updated,
};

template <typename Element>
struct recycle_pool_event final {
    recycle_pool_event_type type;
    std::vector<std::optional<Element>> const &elements;    // fetched, replaced
    std::vector<std::pair<std::size_t, Element>> inserted;  // updated
    std::vector<std::pair<std::size_t, Element>> replaced;  // updated
    std::vector<std::pair<std::size_t, Element>> erased;    // updated
};

template <typename Element>
struct recycle_pool final {
    [[nodiscard]] static std::shared_ptr<recycle_pool<Element>> make_shared();

    /// 全ての要素をそのままセットして更新する
    void replace_all(std::vector<Element> const &);
    /// 元のelementsを活かしながら全ての要素を更新する
    void update_all(std::vector<Element> const &);
    /// 単独の要素を削除する
    void erase(identifier const &);
    void erase_for_id(object_id const &);
    /// 単独の要素を追加する
    void insert(Element const &);
    /// identifierの一致する単独の要素を置き換える
    void replace(Element const &);
    /// identifierの一致する要素があれば置き換え、なければ追加する
    void insert_or_replace(Element const &);

    std::vector<std::optional<Element>> const &elements() const;

    [[nodiscard]] observing::syncable observe_event(std::function<void(recycle_pool_event<Element> const &)> &&);

   private:
    std::vector<std::optional<Element>> _elements;
    observing::fetcher_ptr<recycle_pool_event<Element>> _fetcher;

    recycle_pool();
};
}  // namespace yas::ae

namespace yas {
std::string to_string(ae::recycle_pool_event_type const &);
}  // namespace yas

std::ostream &operator<<(std::ostream &os, yas::ae::recycle_pool_event_type const &value);

#include <audio_editor_core/ae_recycle_pool_private.h>
