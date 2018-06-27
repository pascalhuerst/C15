package com.nonlinearlabs.NonMaps.client.world.overlay;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.google.gwt.canvas.dom.client.Context2d.TextBaseline;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.RGBA;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.pointer.PointerState;

public class ContextMenuItem extends OverlayControl {

	private String text = "";
	private boolean enabled = true;

	public ContextMenuItem(ContextMenu parent, String text, boolean enabled) {
		super(parent);
		this.text = text;
		this.enabled = enabled;
	}

	public String getText() {
		return text;
	}

	public ContextMenuItem(ContextMenu parent, String text) {
		this(parent, text, true);
	}

	@Override
	public Control click(Position eventPoint) {
		if (isEnabled()) {
			Overlay o = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay();
			o.removeExistingContextMenus();
		}
		return this;
	}

	public boolean isEnabled() {
		return enabled;
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
		return this;
	}

	@Override
	public Control mouseUp(Position eventPoint) {
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
		return this;
	}

	@Override
	public ContextMenu getParent() {
		return (ContextMenu) super.getParent();
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {

		double fontHeightInPixels = Millimeter.toPixels(4);

		Rect r = getPixRect().copy();
		double padding = getLeftPadding();

		r.applyPadding(0, padding, 0, padding);

		if (PointerState.get().isCurrentReceiver(this))
			r.fill(ctx, new Gray(51));

		r.applyPadding(padding, 0, padding, 0);

		ctx.setTextAlign(TextAlign.LEFT);

		RGB c = getColorFont();

		if (!isEnabled()) {
			c = new RGBA(c, 0.5);
		}
		ctx.setFillStyle(c.toString());
		ctx.setFont(fontHeightInPixels + "px 'Source Sans Pro LW25'");
		ctx.setTextBaseline(TextBaseline.MIDDLE);
		ctx.fillText(text, r.getLeft(), r.getCenterPoint().getY());
	}

	public double getLeftPadding() {
		return Millimeter.toPixels(1);
	}

	@Override
	public RGB getColorFont() {
		return new Gray(230);
	}

	public double getDesiredWidth() {
		return text.length() * Millimeter.toPixels(2.25);
	}

	public double getDesiredHeight() {
		return Millimeter.toPixels(10);
	}

}
