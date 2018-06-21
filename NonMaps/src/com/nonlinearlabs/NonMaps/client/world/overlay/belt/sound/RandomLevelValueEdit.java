package com.nonlinearlabs.NonMaps.client.world.overlay.belt.sound;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.xml.client.Node;
import com.nonlinearlabs.NonMaps.client.Millimeter;
import com.nonlinearlabs.NonMaps.client.ServerProxy;
import com.nonlinearlabs.NonMaps.client.world.Control;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.overlay.Label;
import com.nonlinearlabs.NonMaps.client.world.overlay.OverlayLayout;

class RandomLevelValueEdit extends Label {
	// private IncrementalChanger changer;

	RandomLevelValueEdit(OverlayLayout parent) {
		super(parent);
	}

	@Override
	public String getDrawText(Context2d ctx) {
		return "";
	}

	@Override
	public Control click(Position eventPoint) {
		/*-
		Rect leftRect = getPixRect().copy();
		leftRect.setRight(getPixRect().getLeft() + getPixRect().getWidth() / 2);
		Rect rightRect = getPixRect().copy();
		rightRect.setLeft(getPixRect().getRight() - getPixRect().getWidth() / 2);
		if (leftRect.contains(eventPoint)) {
			value.dec(Initiator.EXPLICIT_USER_ACTION, false);
			return this;
		} else if (rightRect.contains(eventPoint)) {
			value.inc(Initiator.EXPLICIT_USER_ACTION, false);
			return this;
		}
		-*/
		return super.click(eventPoint);
	}

	@Override
	public Control mouseDown(Position eventPoint) {
		// changer = value.startUserEdit(Millimeter.toPixels(100));
		return this;
	}

	@Override
	public Control mouseUp(Position eventPoint) {
		// changer = null;
		return this;
	}

	@Override
	public Control mouseDrag(Position oldPoint, Position newPoint, boolean fine) {
		double xPix = newPoint.getX() - oldPoint.getX();
		double yPix = oldPoint.getY() - newPoint.getY();
		double pix = xPix;

		if (Math.abs(yPix) > Math.abs(xPix))
			pix = yPix;

		/*-if (changer != null)
			changer.changeBy(fine, pix);
			-*/

		return this;
	}

	@Override
	public Control wheel(Position eventPoint, double amount, boolean fine) {
		/*-
		if (amount > 0)
			value.inc(Initiator.EXPLICIT_USER_ACTION, fine);
		else if (amount < 0)
			value.dec(Initiator.EXPLICIT_USER_ACTION, fine);
		-*/
		return this;
	}

	public void update(Node settings) {
		Node rndAmountNode = ServerProxy.getChild(settings, "RandomizeAmount");
		/*-
		 if (rndAmountNode != null && changer == null) {
		 NodeList nl = rndAmountNode.getChildNodes();
		 int count = nl.getLength();

		 for (int i = 0; i < count; i++) {
		 Node n = nl.item(i);
		 String nodeName = n.getNodeName();

		 if (!nodeName.isEmpty() && !nodeName.startsWith("#")) {
		 value.update(n);

		 if (nodeName.equals("scaling")) {
		 setStringizer(ServerProxy.getText(n));
		 }
		 }
		 }
		 }
		 -*/
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		boolean withArrows = getPixRect().getWidth() >= Millimeter.toPixels(50);
		getPixRect().drawValueEditSliderBackgound(ctx, withArrows, getColorFont());
		super.draw(ctx, invalidationMask);
	}

	@Override
	protected Rect getTextRect() {
		Rect r = super.getTextRect().copy();
		r.reduceWidthBy(Millimeter.toPixels(5));
		return r;
	}
}