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

        InputElement(const InputElement&) = delete;

        InputElement(InputElement&&) noexcept = delete;

        virtual ~InputElement() noexcept;

        InputElement& operator=(const InputElement&) = delete;

        InputElement& operator=(InputElement&&) noexcept = delete;

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
         * \param x X.
         * \param y Y.
         * \return True if point is inside.
         */
        [[nodiscard]] virtual bool intersect(int32_t x, int32_t y) const noexcept = 0;

        ////////////////////////////////////////////////////////////////
        // Events.
        ////////////////////////////////////////////////////////////////

        virtual void onMouseEnter();

        virtual void onMouseExit();

        [[nodiscard]] virtual InputContext::MouseClickResult onMouseClick(InputContext::MouseClick click);
    };
}  // namespace floah
