#ifndef MGL_DRAWABLE_HPP
#define MGL_DRAWABLE_HPP

#include <vector>
#include <functional>

namespace mgl {

using IDrawableVoidCallback = std::function<void()>;

class IDrawable {
public:
	void draw();
	void beforeDraw(IDrawableVoidCallback before);
	void afterDraw(IDrawableVoidCallback after);
	void beforeAndAfterDraw(IDrawableVoidCallback before, IDrawableVoidCallback after);
protected:
	virtual void performDraw() = 0;
private:
	std::vector<IDrawableVoidCallback> beforeDrawCallbacks;
	std::vector<IDrawableVoidCallback> afterDrawCallbacks;

};

}

#endif