package com.nonlinearlabs.NonMaps.client.dataModel;

import java.util.Date;
import java.util.HashMap;
import java.util.function.Function;

public class EditBuffer {

	private static EditBuffer theInstance = new EditBuffer();

	public enum Color {
		green, blue, yellow, orange, purple, red, none;
	}

	private HashMap<String, ParameterGroup> parameterGroups = new HashMap<String, ParameterGroup>();
	private HashMap<String, BasicParameterModel> parameters = new HashMap<String, BasicParameterModel>();

	public StringDataModelEntity selectedParameter = new StringDataModelEntity();
	public StringDataModelEntity loadedPreset = new StringDataModelEntity();

	public EnumDataModelEntity<Color> color = new EnumDataModelEntity<Color>(Color.class, Color.none);
	public StringDataModelEntity comment = new StringDataModelEntity();
	public StringDataModelEntity deviceName = new StringDataModelEntity();
	public DataModelEntity<Date> storeTime = new DateDataModelEntity();

	static public EditBuffer get() {
		return theInstance;
	}

	private EditBuffer() {
	}

	public ParameterGroup getGroup(String id) {
		ParameterGroup g = parameterGroups.get(id);

		if (g == null) {
			g = new ParameterGroup();
			parameterGroups.put(id, g);
		}

		return g;
	}

	public void onParameterChange(int parameterId, Function<ValueDataModelEntity, Boolean> cb) {
		parameters.get(parameterId).value.onChange(cb);
	}

}
