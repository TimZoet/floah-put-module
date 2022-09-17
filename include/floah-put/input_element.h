#pragma once

////////////////////////////////////////////////////////////////
// Standard includes.
////////////////////////////////////////////////////////////////

#include <cstdint>

////////////////////////////////////////////////////////////////
// Current target includes.
////////////////////////////////////////////////////////////////

#include "floah-put/input_context.h"

namespace floah
{
    class InputElement
    {
    public:
        ////////////////////////////////////////////////////////////////
        // Constructors.
        ////////////////////////////////////////////////////////////////

        InputElement();

        InputElement(const InputElement&) = default;

        InputElement(InputElement&&) noexcept = default;

        virtual ~InputElement() noexcept;

        InputElement& operator=(const InputElement&) = default;

        InputElement& operator=(InputElement&&) noexcept = default;

        ////////////////////////////////////////////////////////////////
        // Getters.
        ////////////////////////////////////////////////////////////////

        /**
         * \brief Optional parent for hierarchical input layers.
         * \return Parent or nullptr.
         */
        [[nodiscard]] virtual const InputElement* getInputParent() const noexcept;

        /**
         * \brief Get the input layer. Higher values mean the element is on top of elements in a lower input layer.
         * \return Input layer.
         */
        [[nodiscard]] virtual int32_t getInputLayer() const noexcept;

        /**
         * \brief Offset of this input element in global space.
         * All positions passed to input methods (e.g. the cursor) are translated by the inverse of this value.
         * \return Offset.
         */
        [[nodiscard]] virtual math::int2 getInputOffset() const noexcept;

        ////////////////////////////////////////////////////////////////
        // Input.
        ////////////////////////////////////////////////////////////////

        /**
         * \brief Comparison function for sorting elements by layer.
         * \param other Other element.
         * \return Boolean.
         */
        [[nodiscard]] bool compare(const InputElement& other) const noexcept;

        /**
         * \brief Returns whether point is inside of element.
         * \param point Point.
         * \return True if point is inside.
         */
        [[nodiscard]] virtual bool intersect(math::int2 point) const noexcept = 0;

        ////////////////////////////////////////////////////////////////
        // Events.
        ////////////////////////////////////////////////////////////////

        virtual void onMouseEnter();

        virtual void onMouseExit();

        [[nodiscard]] virtual InputContext::MouseClickResult onMouseClick(InputContext::MouseClick click);

        [[nodiscard]] virtual InputContext::MouseMoveResult onMouseMove(InputContext::MouseMove move);

        [[nodiscard]] virtual InputContext::MouseScrollResult onMouseScroll(InputContext::MouseScroll scroll);
    };
}  // namespace floah
