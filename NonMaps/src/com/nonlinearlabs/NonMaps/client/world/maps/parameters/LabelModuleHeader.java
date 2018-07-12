package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.nonlinearlabs.NonMaps.client.NonMaps;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup;
import com.nonlinearlabs.NonMaps.client.dataModel.Setup.BooleanValues;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Gray;
import com.nonlinearlabs.NonMaps.client.world.Name;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.RGB;
import com.nonlinearlabs.NonMaps.client.world.RGBA;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.maps.parameters.PlayControls.SourcesAndAmounts.SourcesAndAmounts;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenu;
import com.nonlinearlabs.NonMaps.client.world.overlay.ContextMenuItem;
import com.nonlinearlabs.NonMaps.client.world.overlay.Overlay;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

public class LabelModuleHeader extends LabelSmall {

	public class ParameterGroupContextMenu extends ContextMenu {
		public ParameterGroupContextMenu(OverlayLayout parent) {
			super(parent);

			boolean isLocked = isLocked();

			addChild(new ContextMenuItem(this, (isLocked ? "Unlock" : "Lock") + " this Group") {
				@Override
				public Control click(Position eventPoint) {
					toggleLock();
					return super.click(eventPoint);
				}
			});

			boolean isAnyParameterLocked = NonMaps.get().getNonLinearWorld().getParameterEditor().isAnyParameterLocked();
			boolean areAllParametersLocked = NonMaps.get().getNonLinearWorld().getParameterEditor().areAllParametersLocked();

			if (!areAllParametersLocked) {
				addChild(new ContextMenuItem(this, "Lock all Groups") {
					@Override
					public Control click(Position eventPoint) {
						lockAll();
						return super.click(eventPoint);
					}
				});
			}

			if (isAnyParameterLocked) {
				addChild(new ContextMenuItem(this, "Unlock all Groups") {
					@Override
					public Control click(Position eventPoint) {
						unlockAll();
						return super.click(eventPoint);
					}
				});
			}
		}
	}

	public LabelModuleHeader(ParameterGroupVertical parent, Name name) {
		super(parent, name);
	}

	@Override
	public ParameterGroupVertical getParent() {
		return (ParameterGroupVertical) super.getParent();
	}

	protected boolean isLocked() {
		return getParent().isLocked();
	}

	protected void unlockAll() {
		NonMaps.get().getServerProxy().unlockAllGroups();
	}

	protected void lockAll() {
		NonMaps.get().getServerProxy().lockAllGroups();
	}

	protected void toggleLock() {
		NonMaps.get().getServerProxy().toggleGroupLock(getParent().getID());
	}

	public LabelModuleHeader(SourcesAndAmounts parent, String text) {
		super(parent, text);
	}

	@Override
	public void doSecondLayoutPass(double parentsWidthFromFirstPass, double parentsHeightFromFirstPass) {
		setNonSize(parentsWidthFromFirstPass, getNonPosition().getDimension().getHeight());
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		Rect pixRect = getPixRect();
		pixRect.drawRoundedRect(ctx, Rect.ROUNDING_TOP, toXPixels(6), toXPixels(2), new Gray(87), null);
		super.draw(ctx, invalidationMask);

		if (isLocked())
			ctx.fillText("L", pixRect.getRight() - 16, pixRect.getCenterPoint().getY() + toYPixels(moveFontVerticallyBy()));
	}

	@Override
	protected double getFontHeight() {
		return 16;
	}

	@Override
	protected double getBasicWidth() {
		return 70;
	}

	@Override
	protected double getBasicHeight() {
		return 28;
	}

	@Override
	public RGB getColorFont() {
		return new RGBA(getFontColor(), 0.9);
	}

	private RGB getFontColor() {
		return super.getColorSliderHighlight();
	}

	@Override
	protected boolean shouldShowTextForLevelOfDetail(double levelOfDetail) {
		return true;
	}

	@Override
	public double getLevelOfDetailForFullVisibility() {
		return 1;
	}

	@Override
	public Control onContextMenu(Position pos) {
		boolean showContextMenus = Setup.get().localSettings.contextMenus.getValue() == BooleanValues.on;

		if (showContextMenus) {
			Overlay o = NonMaps.theMaps.getNonLinearWorld().getViewport().getOverlay();
			ContextMenu c = new ParameterGroupContextMenu(o);
			return o.setContextMenu(pos, c);
		}
		return super.onContextMenu(pos);
	}
}
