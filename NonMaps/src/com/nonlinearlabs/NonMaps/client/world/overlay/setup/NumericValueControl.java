package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.core.client.JavaScriptObject;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public abstract class NumericValueControl extends OverlayLayout {

	protected CenteredSetupLabel middle;
	// protected QuantizedClippedValue value = new QuantizedClippedValue(this);
	// private QuantizedClippedValue.IncrementalChanger changer = null;
	private JavaScriptObject stringizer;

	public NumericValueControl(Control parent, String initText) {
		super(parent);
		addChild(middle = new CenteredSetupLabel(this, initText));
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		boolean drawArrows = middle.getPixRect().getWidth() >= Millimeter.toPixels(50);
		middle.getPixRect().drawValueEditSliderBackgound(ctx, drawArrows, getColorFont());
		super.draw(ctx, invalidationMask);
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		double yMargin = h / 4;
		middle.doLayout(0, yMargin, w, h - 2 * yMargin);
	}

	void setText(String txt) {
		middle.setText(txt);
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		// changer = value.startUserEdit(Millimeter.toPixels(100));
		return this;
	}

	@Override
	public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
		/*-
		if (changer != null) {
			double xPix = newPoint.getX() - oldPoint.getX();
			double yPix = oldPoint.getY() - newPoint.getY();
			double pix = xPix;

			if (Math.abs(yPix) > Math.abs(xPix))
				pix = yPix;

			changer.changeBy(fine, pix);
		}
		-*/
		return this;
	}

	protected abstract void sendToServer(double newQuantizedValue);

	@Override
	public Control mouseUp(Position eventPoint) {
		return this;
	}

}