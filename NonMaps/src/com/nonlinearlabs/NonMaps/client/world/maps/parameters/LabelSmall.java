package com.nonlinearlabs.NonMaps.client.world.maps.parameters;

import com.google.gwt.canvas.dom.client.Context2d;
import com.google.gwt.canvas.dom.client.Context2d.TextAlign;
import com.google.gwt.canvas.dom.client.Context2d.TextBaseline;
import com.nonlinearlabs.NonMaps.client.Checksum;
import com.nonlinearlabs.NonMaps.client.world.Name;
import com.nonlinearlabs.NonMaps.client.world.Position;
import com.nonlinearlabs.NonMaps.client.world.Rect;
import com.nonlinearlabs.NonMaps.client.world.TextCropper;
import com.nonlinearlabs.NonMaps.client.world.maps.MapsLayout;

public class LabelSmall extends ZoomReactingControl {
	private String text = "";
	private Name name = null;
	private boolean isVisible = false;

	public LabelSmall(MapsLayout parent, Name name) {
		super(parent);
		setName(name);
	}

	public LabelSmall(MapsLayout parent, String text) {
		super(parent);
		setText(text);
	}

	@Override
	public void doFirstLayoutPass(double levelOfDetail) {
		super.doFirstLayoutPass(levelOfDetail);
		isVisible = levelOfDetail >= getLevelOfDetailForFullVisibility();
	}

	public boolean setText(String text) {
		if (this.text.equals(text))
			return false;

		this.text = text;
		requestLayout();
		return true;
	}

	public String getText() {
		return text;
	}

	public void setName(Name name) {
		this.name = name;
		this.text = "";
	}

	protected boolean shouldShowTextForLevelOfDetail(double levelOfDetail) {
		return levelOfDetail >= getLevelOfDetailForFullVisibility();
	}

	protected String getDisplayText() {
		if (name != null) {
			text = name.getLongName();

			if (text.isEmpty())
				text = name.getShortName();
		}
		return text;
	}

	protected Name getName() {
		return name;
	}

	protected double getFontHeight() {
		return 10;
	}

	@Override
	protected double getBasicWidth() {
		return 60;
	}

	@Override
	protected double getBasicHeight() {
		return 15;
	}

	@Override
	protected double getMinHeight() {
		return 0;
	}

	protected double moveFontVerticallyBy() {
		return 0;
	}

	@Override
	public void getStateHash(Checksum crc) {
		super.getStateHash(crc);
		crc.eat(getDisplayText());
		crc.eat(isVisible);
	}

	@Override
	public boolean isVisible() {
		return isVisible && super.isVisible();
	}

	@Override
	public void draw(Context2d ctx, int invalidationMask) {
		if (isVisible) {
			Rect pixRect = getPixRect();

			double fontHeightInUnits = getFontHeight();
			double fontHeightInPixels = toYPixels(fontHeightInUnits);

			TextAlign align = getTextAlignment();
			ctx.setTextAlign(align);
			ctx.setFillStyle(getColorFont().toString());
			ctx.setFont(fontHeightInPixels + "px " + getFontName());

			Position center = pixRect.getCenterPoint();

			switch (align) {
			case LEFT:
				center.setX(pixRect.getLeft());
				break;

			case RIGHT:
				center.setX(pixRect.getRight());
				break;

			default:
				break;
			}

			ctx.setTextBaseline(TextBaseline.MIDDLE);
			String croppedText = TextCropper.crop(getDisplayText(), ctx, pixRect.getWidth() - toXPixels(getWidthMargin()));
			ctx.fillText(croppedText, center.getX(), center.getY() + toYPixels(moveFontVerticallyBy()));
		}
	}

	protected String getFontName() {
		return "nonlinearfont";
	}

	protected TextAlign getTextAlignment() {
		return TextAlign.CENTER;
	}

	@Override
	public double getLevelOfDetailForFullVisibility() {
		return getParent().getLevelOfDetailForFullVisibility() + 1;
	}

}
