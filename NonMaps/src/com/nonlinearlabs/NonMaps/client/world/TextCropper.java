package com.nonlinearlabs.NonMaps.client.world;

import com.google.gwt.canvas.dom.client.Context2d;

public class TextCropper {
	public static String crop(String text, Context2d ctx, double maxWidth) {
		int min = 0;
		int max = text.length();

		if (ctx.measureText(text).getWidth() <= maxWidth)
			return text;

		return customAlgorithmAlaJustus(text, min, max, ctx, maxWidth);
	}

	private static String customAlgorithmAlaJustus(String text, int min, int max, Context2d ctx, double maxWidth) {
		if (max == 0)
			return "";
		if ((min + 1) == max)
			return text.substring(0, min) + "..";
		int roughTwoThirds = (text.length() / 3) * 2;
		int lastSpace = text.lastIndexOf(" ");
		if (lastSpace >= roughTwoThirds) {
			String partAfterSpace = text.substring(lastSpace + 1);
			StringBuilder front = new StringBuilder();
			for (int i = 0;
				 i < max - 2 - partAfterSpace.length() && ctx.measureText(front + partAfterSpace + "---").getWidth() < maxWidth;
				 i++) {
				front.append(text.charAt(i));
			}
			return front + ".." + partAfterSpace;
		} else {
			return binarySearchLength(text, min, max, ctx, maxWidth);
		}
	}


		private static String binarySearchLength(String text, int min, int max, Context2d ctx, double maxWidth) {
		if (max == 0)
			return "";

		if ((min + 1) == max)
			return text.substring(0, min) + "..";

		int halfIdx = min + (max - min) / 2;
		String half = text.substring(0, halfIdx);
		String toMeasure = half + "..";

		if (ctx.measureText(toMeasure).getWidth() <= maxWidth)
			min = halfIdx;
		else
			max = halfIdx;

		return binarySearchLength(text, min, max, ctx, maxWidth);
	}

}
