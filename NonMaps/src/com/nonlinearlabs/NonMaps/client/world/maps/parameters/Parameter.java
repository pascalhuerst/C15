package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Checksum;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.Tracer;
import com.nonlinearlabs.NonMaps.client.presenters.ParameterPresenter;
import com.nonlinearlabs.NonMaps.client.presenters.ParameterPresenterProviders;
import com.nonlinearlabs.NonMaps.client.useCases.EditBuffer;
import com.nonlinearlabs.NonMaps.client.useCases.IncrementalChanger;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.LayoutResizingVertical;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsControl;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.setup.ContextMenusSetting;
import com.nonlinearlabs.NonMaps.client.world.overlay.setup.EditParameterSetting;
import com.nonlinearlabs.NonMaps.client.world.pointer.TouchPinch;

public abstract class Parameter extends LayoutResizingVertical {

	private int id;
	static private final ParameterPresenter defaultPresenter = new ParameterPresenter();
	private ParameterPresenter presenter = defaultPresenter;
	private IncrementalChanger currentParameterChanger;

	public Parameter(MapsLayout parent) {
		super(parent);
	}

	@Override
	public void init() {
		super.init();
		ParameterPresenterProviders.get().register(getParameterID(), p -> updateUI(p));
	}

	public ParameterPresenter getPresenter() {
		return presenter;
	}

	public ParameterEditor getSelectionRoot() {
		return NonMaps.get().getNonLinearWorld().getParameterEditor();
	}

	private boolean updateUI(ParameterPresenter p) {
		presenter = p;
		invalidate(INVALIDATION_FLAG_UI_CHANGED);
		return true;
	}

	public int getParameterID() {
		return id;
	}

	@Override
	public Control click(Position eventPoint) {
		boolean isAnyChildVisible = isAnyChildVisible();

		if (!isAnyChildVisible)
			return null;

		EditBuffer.get().selectParameter(getParameterID());
		return this;
	}

	@Override
	public Control doubleClick() {
		EditBuffer.get().setParameterDefault(getParameterID());
		return this;
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		EditParameterSetting s = getWorld().getViewport().getOverlay().getSetup().getEditParameterDragSetting();
		String choiceString = s.getSettingsControl().getChoiceString();

		switch (choiceString) {
		case "Always":
			EditBuffer.get().selectParameter(getParameterID());
			startEdit();
			return this;

		case "If Selected":
			if (getPresenter().selected) {
				startEdit();
				return this;
			}

			break;

		case "Never":
			break;

		default:
			break;
		}

		return super.mouseDown(eventPoint);
	}

	public void startEdit() {
		if (getPresenter().isBoolean) {
			EditBuffer.get().toggleBooleanParameter(getParameterID());
		} else {
			currentParameterChanger = EditBuffer.get().startUserEdit(getParameterID(), Millimeter.toPixels(100));
		}
	}

	@Override
	public boolean wantsMouseUpAnimation() {
		return false;
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {

		getPixRect().drawRoundedRect(ctx, getBackgroundRoundings(), toXPixels(4), 0, getParameterBackgroundColor(), null);

		super.draw(ctx, invalidationMask);

		if (getPresenter().selected)
			getPixRect().drawRoundedRect(ctx, getBackgroundRoundings(), toXPixels(4), toXPixels(1), null, getColorSliderHighlight());
	}

	private RGB getParameterBackgroundColor() {
		return getPresenter().selected ? getColorObjectBackgroundSelected() : new Gray(52);
	}

	@Override
	public Control pinch(Position eventPoint, double touchDist, TouchPinch pinch) {
		if (getPresenter().selected) {
			Tracer.log("Parameter.pinch");
			return NonMaps.theMaps.getNonLinearWorld();
		}
		return super.pinch(eventPoint, touchDist, pinch);
	}

	@Override
	public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
		EditParameterSetting s = getWorld().getViewport().getOverlay().getSetup().getEditParameterDragSetting();
		boolean noDrag = s.getSettingsControl().getChoiceString().equals("Never") || getWorld().isSpaceDown();

		if (getPresenter().selected && !noDrag) {

			double xPix = newPoint.getX() - oldPoint.getX();
			double yPix = oldPoint.getY() - newPoint.getY();
			double pix = xPix;

			if (Math.abs(yPix) > Math.abs(xPix))
				pix = yPix;

			if (currentParameterChanger != null)
				currentParameterChanger.changeBy(fine, pix);

			return this;
		} else if (noDrag) {
			return getWorld().mouseDrag(oldPoint, newPoint, fine);
		}

		return null;
	}

	@Override
	public void onMouseLost() {
		if (currentParameterChanger != null) {
			currentParameterChanger.finish();
			currentParameterChanger = null;
		}
		super.onMouseLost();
	}

	private boolean isAnyChildVisible() {
		boolean isAnyChildVisible = false;

		for (MapsControl c : getChildren()) {
			isAnyChildVisible |= c.isVisible();

			if (isAnyChildVisible)
				break;
		}
		return isAnyChildVisible;
	}

	public enum Initiator {
		EXPLICIT_USER_ACTION, INDIRECT_USER_ACTION, MODULATION
	}

	public String getParameterGroupID() {
		return getParameterGroup().getID();
	}

	public boolean shouldHaveHandleOnly() {
		return false;
	}

	public ContextMenu createContextMenu(Overlay o) {
		return null;
	}

	public boolean hasContextMenu() {
		return false;
	}

	public boolean dimHandleAtDefaultValue() {
		return false;
	}

	public boolean isOracle() {
		return true;
	}

	public static Parameter findInParents(MapsControl c) {
		if (c instanceof Parameter)
			return (Parameter) c;

		return findInParents(c.getParent());
	}

	public ParameterGroupIface getParameterGroup() {
		Control p = getParent();
		while (p != null) {
			if (p instanceof ParameterGroupIface)
				return (ParameterGroupIface) p;

			p = p.getParent();
		}
		return null;
	}

	@Override
	public Control onContextMenu(Position pos) {
		EditBuffer.get().selectParameter(id);

		// select(Initiator.EXPLICIT_USER_ACTION);

		ContextMenusSetting contextMenuSettings = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay().getSetup()
				.getContextMenuSettings();
		if (contextMenuSettings.isEnabled()) {
			if (hasContextMenu()) {
				Overlay o = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay();
				return o.setContextMenu(pos, createContextMenu(o));
			}
		}
		return super.onContextMenu(pos);
	}

	public String getFullNameWithGroup() {
		// return getGroupName() + "   \u2013   " +
		// getDataModel().getName().getLongName();
		return "";
	}

	public String getGroupName() {
		ParameterGroupIface group = (ParameterGroupIface) getParameterGroup();
		return group.getName().getLongName();
	}

	protected int getBackgroundRoundings() {
		return Rect.ROUNDING_NONE;
	}

	@Override
	public void getStateHash(Checksum crc) {
		super.getStateHash(crc);
		presenter.getHash(crc);
	}

}
