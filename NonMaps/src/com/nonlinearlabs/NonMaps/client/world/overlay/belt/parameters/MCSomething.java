package com.nonlinearlabs.NonMaps.client.world.overlay.belt.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGBA;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.ModulatableParameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.Parameter;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.MacroControls.Macros.MacroControls;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public abstract class MCSomething extends OverlayLayout {

	protected OverlayControl middle;

	public MCSomething(Control parent) {
		super(parent);
	}

	public OverlayControl getMiddle() {
		return middle;
	}

	@Override
	public BeltParameterLayout getParent() {
		return (BeltParameterLayout) super.getParent();
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {

		boolean drawArrows = middle.getPixRect().getWidth() >= Millimeter.toPixels(50);

		if (middle.getPixRect().getWidth() > Millimeter.toPixels(25))
			middle.getPixRect().drawValueEditSliderBackgound(ctx, drawArrows, getColorFont());

		super.draw(ctx, invalidationMask);

		if (shouldDrawShadow()) {
			getPixRect().fill(ctx, new RGBA(getColorModuleBackground(), 0.5));
		}
	}

	protected boolean shouldDrawShadow() {

		Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();

		if (p instanceof ModulatableParameter) {
			ModulatableParameter m = (ModulatableParameter) p;
			if (m.getModulationSource() != MacroControls.NONE)
				return false;
		}
		return true;
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		double maxWidth = Millimeter.toPixels(60);
		double xMargin = 0;
		double yMargin = h / 8;

		if (w > maxWidth) {
			xMargin = (w - maxWidth) / 2;
			w = maxWidth;
		}

		super.doLayout(x + xMargin, y + yMargin, w, h - 2 * yMargin);
		middle.doLayout(0, 0, getRelativePosition().getWidth(), getRelativePosition().getHeight());
	}

	@Override
	public Control wheel(Position eventPoint, double amount, boolean fine) {
		Parameter p = getNonMaps().getNonLinearWorld().getParameterEditor().getSelectedOrSome();

		if (p instanceof ModulatableParameter) {
			ModulatableParameter m = (ModulatableParameter) p;

			if (amount > 0)
				inc(m, fine);
			else if (amount < 0)
				dec(m, fine);
		}

		return this;
	}

	protected abstract void dec(ModulatableParameter m, boolean fine);

	protected abstract void inc(ModulatableParameter m, boolean fine);

}