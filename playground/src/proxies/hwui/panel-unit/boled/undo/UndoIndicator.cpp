#include <Application.h>
#include "UndoIndicator.h"
#include <http/UndoScope.h>

UndoIndicator::UndoIndicator(const Rect &pos) : Label("\ue20b",
                                                      [=]() {
                                                          auto rect = pos;
                                                          rect.setWidth(8);
                                                          rect.setHeight(7);
                                                          return rect;
                                                      }())
                                                              {
    Application::get().getUndoScope()->onUndoScopeChanged(mem_fun(this, &UndoIndicator::setDirty));

}

bool UndoIndicator::redraw(FrameBuffer &fb) {
    if (Application::get().getUndoScope()->canRedo()) {
        Label::redraw(fb);
    }
    return true;
}