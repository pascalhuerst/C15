package com.nonlinearlabs.NonMaps.client.world.overlay.setup;

import com.google.gwt.xml.client.Node;

public class SoftwareUpdateAvailableSettings extends SettingsControl {

	public class MyLabel extends SetupLabel {

		public MyLabel(SoftwareUpdateAvailableSettings parent) {
			super(parent, "");
		}
	}

	public class MyButton extends SetupButton {

		public MyButton(SoftwareUpdateAvailableSettings parent) {
			super(parent, "No Update");
		}

		public void applyPadding() {
			double hPadding = (getRelativePosition().getHeight() / 2);
			getRelativePosition().applyPadding(0, hPadding / 2, 0, hPadding / 2);
		}
	}

	private MyLabel label;
	private MyButton button;

	protected SoftwareUpdateAvailableSettings(SoftwareUpdateAvailable parent) {
		super(parent);
		addChild(label = new MyLabel(this));
		addChild(button = new MyButton(this));
	}

	@Override
	public void update(Node settingsNode, Node deviceInfo) {
	}

	@Override
	public void doLayout(double x, double y, double w, double h) {
		super.doLayout(x, y, w, h);
		double margin = getButtonDimension() / 2;
		double elementsWidth = (w - margin);
		label.doLayout(0, 0, 3 * elementsWidth / 5, h);
		button.doLayout(label.getRelativePosition().getWidth() + margin, 0, 2 * elementsWidth / 5, h);
		button.applyPadding();
	}

}
