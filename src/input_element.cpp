#include "floah-put/input_element.h"

#include <utility>

namespace
{
    /**
     * \brief Traverse up the hierarchy to find the nearest common ancestor of two elements.
     * It is possible that one of the elements is a descendant of the other, in which case we
     * must go up only on one side. This is indicated with the balance.
     * \param lhs Left element.
     * \param rhs Right element.
     * \return {balance, comparison} (comparison is only valid when balance == 0).
     */
    [[nodiscard]] std::pair<int32_t, bool> compareElements(const floah::InputElement& lhs,
                                                           const floah::InputElement& rhs)
    {
        const auto* p0 = lhs.getInputParent();
        const auto* p1 = rhs.getInputParent();

        // Found common ancestor, compare direct descendants (which may or may not be the original input elements).
        if (p0 == p1) return {0, lhs.getInputLayer() > rhs.getInputLayer()};

        if (p0 && p1)
        {
            const auto [balance, comp] = compareElements(*p0, *p1);

            // Compare lhs and parent(rhs).
            if (balance == -1) return compareElements(lhs, *p1);

            // Compare parent(lhs) and rhs.
            if (balance == 1) return compareElements(*p0, rhs);

            // Done comparing, propagate result down.
            return {0, comp};
        }

        // Ran out of parents on the left side, previous comparison must compare lhs and parent(rhs).
        if (p0 == nullptr) return {-1, false};

        // Ran out of parents on the right side, previous comparison must compare parent(lhs) and rhs.
        return {1, false};
    }
}  // namespace

namespace floah
{
    ////////////////////////////////////////////////////////////////
    // Constructors.
    ////////////////////////////////////////////////////////////////

    InputElement::InputElement() = default;

    InputElement::~InputElement() noexcept = default;

    ////////////////////////////////////////////////////////////////
    // Getters.
    ////////////////////////////////////////////////////////////////

    const InputElement* InputElement::getInputParent() const noexcept { return nullptr; }

    int32_t InputElement::getInputLayer() const noexcept { return 0; }

    ////////////////////////////////////////////////////////////////
    // Input.
    ////////////////////////////////////////////////////////////////

    bool InputElement::compare(const InputElement& other) const noexcept
    {
        const auto [balance, comp] = compareElements(*this, other);
        // Other is an ancestor of this and must be placed on top.
        if (balance == 1) return true;
        // This is an ancestor of other.
        if (balance == -1) return false;

        return comp;
    }

    ////////////////////////////////////////////////////////////////
    // Events.
    ////////////////////////////////////////////////////////////////

    void InputElement::onMouseEnter() {}

    void InputElement::onMouseExit() {}

    InputContext::MouseClickResult InputElement::onMouseClick(InputContext::MouseClick)
    {
        return InputContext::MouseClickResult{};
    }

}  // namespace floah
