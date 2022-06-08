#include "floah-put/input_element.h"

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
        const auto* p0 = getInputParent();
        const auto* p1 = other.getInputParent();

        // Both elements have a parent.
        if (p0 && p1)
        {
            // Elements have a common parent, compare layers.
            if (p0 == p1) return getInputLayer() > other.getInputLayer();

            // Traverse up.
            return p0->compare(*p1);
        }

        // Compare parent of this element with other element.
        if (p0) return p0->compare(other);

        // Compare this element with parent of other element.
        if (p1) return compare(*p1);

        // No more parents, compare layers.
        return getInputLayer() > other.getInputLayer();
    }

    ////////////////////////////////////////////////////////////////
    // Events.
    ////////////////////////////////////////////////////////////////

    void InputElement::onMouseEnter() const {}

    void InputElement::onMouseExit() const {}

}  // namespace floah
