#ifndef MGL_DRAWABLE_HPP
#define MGL_DRAWABLE_HPP

#include <vector>
#include <functional>

namespace mgl {

	using IDrawableVoidCallback = std::function<void()>;

	/// <summary>
	/// Represents any entity that need to be drawn into the screen.
	/// Allows to define before and after drawing logic, usually needed
	/// to set different openGL state before drawing some object, and 
	/// reseting it to default afterwards.
	/// </summary>
	class IDrawable {
	public:
		/// <summary>
		/// Applies before and after draw logic if any,
		/// and calls 'performDraw' for the entity to be processed
		/// </summary>
		void draw();

		/// <summary>
		/// Defines logic to run before an entitity is processed by OpenGL.
		/// Usually used to set custom OpenGL state before drawing the entitiy.
		/// </summary>
		void beforeDraw(IDrawableVoidCallback before);

		/// <summary>
		/// Defines logic to run after an entitity is processed by OpenGL.
		/// Usually used to reset OpenGL state previously set by the 'beforeDraw' call.
		/// </summary>
		void afterDraw(IDrawableVoidCallback after);

		/// <summary>
		/// Defines both before and after callbacks to set OpenGL state before
		/// drawing an entity, and also logic for reseting the OpenGL state after
		/// the entity is drawn.
		/// </summary>
		void beforeAndAfterDraw(IDrawableVoidCallback before, IDrawableVoidCallback after);

	protected:
		/// <summary>
		/// Defines the actual drawing logic. This function is called by the public 'draw'
		/// call after any 'beforeDraw' functionality has been called. 
		/// </summary>
		virtual void performDraw() = 0;


	private:
		// TODO - define an array instead of vector -> vectors incur an 
		// additional indirection - arrays avoid this at cost of limited size.
		// Maybe even use only a single callback instead of an array of callbacks
		std::vector<IDrawableVoidCallback> beforeDrawCallbacks;
		std::vector<IDrawableVoidCallback> afterDrawCallbacks;

	};

}

#endif