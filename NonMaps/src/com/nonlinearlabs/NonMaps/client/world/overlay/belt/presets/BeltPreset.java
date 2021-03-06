package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.StoreSelectMode;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.IBank;
import com.nonlinearlabs.NonMaps.client.world.IPreset;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.presets.bank.preset.Preset;
import com.nonlinearlabs.NonMaps.client.world.overlay.DragProxy;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.EditBufferDraggingButton;

public class BeltPreset extends OverlayLayout implements IPreset {

	public enum DropPosition {
		NONE, TOP, MIDDLE, BOTTOM
	}

	private Preset mapsPreset;
	private OverlayControl color;
	private PresetNumber number;
	private PresetName name;
	private DropPosition dropPosition = DropPosition.NONE;

	protected BeltPreset(PresetList parent, Preset mapsPreset) {
		super(parent);
		setOrigin(mapsPreset);

		color = addChild(new PresetColorTag(this));
		number = addChild(new PresetNumber(this));
		name = addChild(new PresetName(this));
		name.setFontHeightInMM(4.5);
		number.setFontHeightInMM(4.5);
	}

	@Override
	public PresetList getParent() {
		return (PresetList) super.getParent();
	}

	public Preset getMapsPreset() {
		return mapsPreset;
	}

	public void setOrigin(Preset mapsPreset) {
		if (this.mapsPreset != mapsPreset) {
			this.mapsPreset = mapsPreset;
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);

		double numberWidth = Millimeter.toPixels(10);
		double xSpace = Millimeter.toPixels(5);

		color.doLayout(2, 0, 7, h);
		number.doLayout(7, 0, numberWidth, h);
		name.doLayout(numberWidth + xSpace, 0, w - (numberWidth + xSpace), h);
	}

	private boolean isInStoreMode() {
		return NonMaps.get().getNonLinearWorld().getPresetManager().isInStoreSelectMode();
	}

	private StoreSelectMode getStoreMode() {
		return NonMaps.get().getNonLinearWorld().getPresetManager().getStoreSelectMode();
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		boolean loaded = mapsPreset.isLoaded() && !mapsPreset.isInStoreSelectMode();
		boolean selected = mapsPreset.isSelected() || mapsPreset.isContextMenuActiveOnMe();
		boolean isOrignalPreset = false;

		if (isInStoreMode()) {
			isOrignalPreset = getStoreMode().isOriginalPreset(mapsPreset);
		}

		RGB colorFill = new RGB(25, 25, 25);

		if (selected && !isOrignalPreset)
			colorFill = new RGB(77, 77, 77);

		if (loaded || isOrignalPreset)
			colorFill = RGB.blue();

		getPixRect().fill(ctx, colorFill);

		super.draw(ctx, invalidationMask);
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		return this;
	}

	@Override
	public Control mouseUp(Position eventPoint) {
		StoreSelectMode storeMode = getNonMaps().getNonLinearWorld().getPresetManager().getStoreSelectMode();
		if (storeMode != null) {
			storeMode.setSelectedPreset(mapsPreset);
			return this;
		}

		if (mapsPreset.isSelected())
			mapsPreset.load();
		else
			mapsPreset.select();

		getParent().scheduleAutoScroll(PresetList.ScrollRequest.Smooth);
		return this;
	}

	public DropPosition getDropPosition() {
		return dropPosition;
	}

	@Override
	public Control startDragging(Position pos) {
		return getNonMaps().getNonLinearWorld().getViewport().getOverlay().createDragProxy(this);
	}

	@Override
	public Control drag(Position pos, DragProxy dragProxy) {
		if (!getPixRect().contains(pos))
			return null;

		if (dragProxy.getOrigin() instanceof IPreset || dragProxy.getOrigin() instanceof EditBufferDraggingButton
				|| dragProxy.getOrigin() instanceof IBank) {
			Rect r = getPixRect();
			if (pos.getY() < r.getTop() + r.getHeight() * 0.25)
				setDropPosition(DropPosition.TOP);
			else if (pos.getY() < r.getTop() + r.getHeight() * 0.75)
				setDropPosition(DropPosition.MIDDLE);
			else
				setDropPosition(DropPosition.BOTTOM);

			return this;
		}
		return super.drag(pos, dragProxy);
	}

	@Override
	public void dragLeave() {
		setDropPosition(DropPosition.NONE);
		super.dragLeave();
	}

	private void setDropPosition(DropPosition p) {
		if (dropPosition != p) {
			dropPosition = p;
			invalidate(INVALIDATION_FLAG_UI_CHANGED);
		}
	}

	@Override
	public Control drop(Position pos, DragProxy dragProxy) {
		if (dragProxy.getOrigin() instanceof IPreset)
			insertPreset(dragProxy);
		else if (dragProxy.getOrigin() instanceof EditBufferDraggingButton)
			insertEditBuffer();
		else if (dragProxy.getOrigin() instanceof IBank)
			insertBank((IBank) dragProxy.getOrigin());

		setDropPosition(DropPosition.NONE);
		return this;
	}

	protected void insertPreset(DragProxy dragProxy) {
		Preset p = mapsPreset;
		IPreset newPreset = (IPreset) dragProxy.getOrigin();

		boolean isMove = p.getParent().getPresetList().findPreset(newPreset.getUUID()) != null;

		if (isMove)
			movePreset(p, newPreset);
		else
			copyPreset(p, newPreset);
	}

	protected void insertBank(IBank bank) {
		Preset p = mapsPreset;

		switch (dropPosition) {
		case TOP:
			getNonMaps().getServerProxy().insertBankAbove(bank, p);
			break;

		case MIDDLE:
			getNonMaps().getServerProxy().overwritePresetWithBank(bank, p);
			break;

		case BOTTOM:
			getNonMaps().getServerProxy().insertBankBelow(bank, p);
			break;

		default:
			break;
		}
	}

	protected void insertEditBuffer() {
		Preset p = mapsPreset;

		switch (dropPosition) {
		case TOP:
			getNonMaps().getServerProxy().insertEditBufferAbove(p);
			break;

		case MIDDLE:
			getNonMaps().getServerProxy().overwritePresetWithEditBuffer(p);
			break;

		case BOTTOM:
			getNonMaps().getServerProxy().insertEditBufferBelow(p);
			break;

		default:
			break;
		}
	}

	protected void copyPreset(Preset p, IPreset newPreset) {
		switch (dropPosition) {
		case TOP:
			getNonMaps().getServerProxy().insertPresetCopyAbove(newPreset, p);
			break;

		case MIDDLE:
			getNonMaps().getServerProxy().overwritePresetWith(newPreset, p);
			break;

		case BOTTOM:
			getNonMaps().getServerProxy().insertPresetCopyBelow(newPreset, p);
			break;

		default:
			break;
		}
	}

	protected void movePreset(Preset p, IPreset newPreset) {
		switch (dropPosition) {
		case TOP:
			getNonMaps().getServerProxy().movePresetAbove(newPreset, p);
			break;

		case MIDDLE:
			getNonMaps().getServerProxy().movePresetTo(newPreset, p);
			break;

		case BOTTOM:
			getNonMaps().getServerProxy().movePresetBelow(newPreset, p);
			break;

		default:
			break;
		}
	}

	@Override
	public String getUUID() {
		return mapsPreset.getUUID();
	}

	@Override
	public Control onContextMenu(Position pos) {
		if (mapsPreset.isInStoreSelectMode())
			return null;

		if (mapsPreset != null) {
			Overlay o = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay();
			return o.setContextMenu(pos, new PresetContextMenu(o, mapsPreset));
		}
		return super.onContextMenu(pos);
	}

	public void rename() {
		if (mapsPreset != null)
			mapsPreset.rename();

	}

}
