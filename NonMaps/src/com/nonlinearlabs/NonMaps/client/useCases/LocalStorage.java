package com.nonlinearlabs.NonMaps.client.useCases;

public interface LocalStorage {
	public void store(String key, String value);

	public String load(String key, String defaultValue);
}
