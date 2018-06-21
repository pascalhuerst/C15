package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.SVGImage;

public class ContextMenuButton extends SVGImage {

	public ContextMenuButton(Control parent) {
		super(parent, "Menu_A_Enabled.svg");
	}

	private Control open(Position pos) {
		/*-
		Overlay o = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay();
		Parameter p = NonMaps.theMaps.getNonLinearWorld().getParameterEditor().getSelection();
		ContextMenu m = p.createContextMenu(o);
		o.setContextMenu(pos, m);
		return m;
		-*/
		return null;
	}

	@Override
	public Control mouseDown(Position pos) {
		Control m = open(pos);
		if (m != null)
			return m;
		return super.mouseDown(pos);
	}

	@Override
	public Control onContextMenu(Position pos) {
		Control m = open(pos);
		if (m != null)
			return m;
		return super.mouseDown(pos);
	};
}
