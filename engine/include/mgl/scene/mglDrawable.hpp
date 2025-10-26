#pragma once

#include <vector>
#include <functional>

namespace mgl {

	using IDrawableVoidCallback = std::function<void()>;

	/**
	 * @brief Represents any entity that needs to be drawn to the screen.
	 *
	 * Allows defining logic to execute before and after drawing, typically
	 * to modify OpenGL state prior to rendering an object and restore it
	 * afterwards.
	 */
	class IDrawable {
	public:
		/**
		 * @brief Executes drawing for the entity.
		 *
		 * This method applies any registered "before" and "after" callbacks
		 * and invokes the @ref performDraw() method that defines the actual
		 * OpenGL drawing logic.
		 */
		void draw();

		/**
		 * @brief Registers a callback to run before the entity is drawn.
		 *
		 * Typically used to configure custom OpenGL state before issuing
		 * rendering commands for this entity.
		 *
		 * @param before The function to execute before drawing.
		 */
		void beforeDraw(IDrawableVoidCallback before);

		/**
		 * @brief Registers a callback to run after the entity is drawn.
		 *
		 * Typically used to restore OpenGL state that was modified in
		 * @ref beforeDraw().
		 *
		 * @param after The function to execute after drawing.
		 */
		void afterDraw(IDrawableVoidCallback after);

		/**
		 * @brief Registers both pre- and post-draw callbacks.
		 *
		 * Combines the effects of @ref beforeDraw() and @ref afterDraw()
		 * in a single call.
		 *
		 * @param before The function to execute before drawing.
		 * @param after  The function to execute after drawing.
		 */
		void beforeAndAfterDraw(IDrawableVoidCallback before, IDrawableVoidCallback after);

	protected:
		/**
		 * @brief Defines the actual drawing logic.
		 *
		 * Called internally by @ref draw() after executing all "before"
		 * callbacks and before executing "after" callbacks.
		 */
		virtual void performDraw() = 0;

	private:
		// TODO: Consider replacing vectors with fixed-size arrays to avoid
		// the extra indirection cost of dynamic storage. Alternatively,
		// collapse to a single callback if multiple are unnecessary.
		std::vector<IDrawableVoidCallback> beforeDrawCallbacks;
		std::vector<IDrawableVoidCallback> afterDrawCallbacks;
	};

} // namespace mgl
