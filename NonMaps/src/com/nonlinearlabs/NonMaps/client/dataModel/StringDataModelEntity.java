package com.nonlinearlabs.NonMaps.client.dataModel;

public class StringDataModelEntity extends DataModelEntity<String> {
	public StringDataModelEntity() {
		super("");
	}

	@Override
	public void fromString(String str) {
		setValue(str);
	}
}