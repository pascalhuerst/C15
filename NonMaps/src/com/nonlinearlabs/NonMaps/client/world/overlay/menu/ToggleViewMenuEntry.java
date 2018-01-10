package com.nonlinearlabs.NonMaps.client.world.overlay.menu;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.FloatingWindow;

public class ToggleViewMenuEntry extends MenuEntry {

	private FloatingWindow window;

	public ToggleViewMenuEntry(GlobalMenu menu, FloatingWindow wnd) {
		super(menu);
		window = wnd;
	}

	@Override
	public String getDrawText(Context2d ctx) {
		return (window.isShown() ? "Hide " : "Show ") + window.getMenuTitle();
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		window.toggle();
		getParent().toggle();
		return this;
	}

}
