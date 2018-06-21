package com.nonlinearlabs.NonMaps.client.presenters;

public class ParameterPresenter {
	public class Range {
		public double left = 0;
		public double value = 0;
		public double right = 0;
	}

	public String displayValue = "---";
	public Range valueRange = new Range();
	public boolean bipolar = false;
	public boolean selected = false;
	public boolean locked = false;
	public String hash = "";
	public boolean isBoolean = false;
	public String shortName = "---";

	public static class Modulation {
		static public String[] sourceOptions = { "", "Ⓐ", "Ⓑ", "Ⓒ", "Ⓓ" };
		static int selectedSourceOption = 0;
		public Range modulationRange = null;
	}
}
