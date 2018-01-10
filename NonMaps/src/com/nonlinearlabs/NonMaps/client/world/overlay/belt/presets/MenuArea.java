package com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets;

import java.util.Iterator;

import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayControl;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;
import com.nonlinearlabs.NonMaps.client.world.overlay.belt.presets.MenuAreaButton.ImageSelection;

class MenuArea extends OverlayLayout {

	MenuArea(BeltPresetLayout parent) {
		super(parent);

		addChild(new MenuAreaPresetButton(this));
		addChild(new MenuAreaBankButton(this));
		addChild(new MenuAreaSearchButton(this));
		addChild(new MenuAreaInfoButton(this));
	}

	@Override
	public BeltPresetLayout getParent() {
		return (BeltPresetLayout) super.getParent();
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		double margin = getButtonDimension() / 2;
		double largeButtonWidth = getLargeButtonWidth();
		double smallButtonWidth = getSmallButtonWidth();
		double widthForLargeGrid = 2 * largeButtonWidth + margin;
		double widthForLargeStack = largeButtonWidth;
		double widthForSmallGrid = 2 * smallButtonWidth + margin;
		double widthForSmallStack = smallButtonWidth;

		if (w >= widthForLargeGrid) {
			super.doLayout(x, y, widthForLargeGrid, h);
			layoutLargeGrid(widthForLargeGrid, h, largeButtonWidth);
		} else if (w >= widthForSmallGrid) {
			super.doLayout(x, y, widthForSmallGrid, h);
			layoutSmallGrid(widthForSmallGrid, h, smallButtonWidth);
		} else if (w >= widthForLargeStack) {
			super.doLayout(x, y, widthForLargeStack, h);
			layoutLargeStack(widthForLargeStack, h, largeButtonWidth);
		} else {
			super.doLayout(x, y, widthForSmallStack, h);
			layoutSmallStack(widthForSmallStack, h, smallButtonWidth);
		}
	}

	private double getSmallButtonWidth() {
		return Millimeter.toPixels(10);
	}

	private double getLargeButtonWidth() {
		return Millimeter.toPixels(31);
	}

	private void layoutSmallStack(double w, double h, double smallButtonWidth) {
		layoutStack(w, h, smallButtonWidth, ImageSelection.Small);
	}

	private void layoutLargeStack(double w, double h, double largeButtonWidth) {
		layoutStack(w, h, largeButtonWidth, ImageSelection.Large);
	}

	private void layoutSmallGrid(double w, double h, double smallButtonWidth) {
		layoutGrid(w, h, smallButtonWidth, ImageSelection.Small);
	}

	private void layoutLargeGrid(double w, double h, double largeButtonWidth) {
		layoutGrid(w, h, largeButtonWidth, ImageSelection.Large);
	}

	private void layoutGrid(double w, double h, double buttonWidth, ImageSelection imageSelection) {
		double margin = getButtonDimension() / 2;
		int numChildren = 4;
		Iterator<OverlayControl> it = getChildren().iterator();

		int numCols = 2;
		int childrenPerRow = numChildren / numCols;
		double xPos = 0;
		double YMargins = (childrenPerRow - 1) * margin;

		for (int col = 0; col < numCols; col++) {

			double yPos = 0;

			for (int row = 0; row < childrenPerRow; row++) {
				double heightPerChild = (h - YMargins) / childrenPerRow;

				if (it.hasNext()) {
					MenuAreaButton button = (MenuAreaButton) it.next();
					button.doLayout(xPos, yPos, buttonWidth, heightPerChild, imageSelection);
					yPos += heightPerChild + margin;
				}
			}

			xPos += buttonWidth + margin;
		}
	}

	private void layoutStack(double w, double h, double buttonWidth, ImageSelection selection) {
		int numChildren = 4;
		double margin = getButtonDimension() / 2;
		double margins = (numChildren - 1) * margin;
		double heightPerChild = (h - margins) / numChildren;
		double yPos = 0;

		for (OverlayControl c : getChildren()) {
			MenuAreaButton button = (MenuAreaButton) c;
			button.doLayout(0, yPos, buttonWidth, heightPerChild, selection);
			yPos += heightPerChild + margin;
		}
	}

}
