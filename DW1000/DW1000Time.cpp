/*
 * Copyright (c) 2015 by Thomas Trojer <thomas@trojer.net>
 * Decawave DW1000 library for Arduino.
 *
 * This file is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */

#include "DW1000Time.h"

DW1000Time::DW1000Time() {
	_timestamp = 0;
}

DW1000Time::DW1000Time(byte data[]) {
	setTimestamp(data);
}

DW1000Time::DW1000Time(float timeUs) {
	setTime(timeUs);
}

DW1000Time::DW1000Time(const DW1000Time& copy) {
	_timestamp = copy.getTimestamp();
}

DW1000Time::DW1000Time(unsigned long value, float factorUs) {
	float tsValue = value * factorUs;
	tsValue = fmod(tsValue, TIME_OVERFLOW);
	setTime(tsValue);
}

DW1000Time::~DW1000Time() {}

void DW1000Time::setTime(float timeUs) {
	_timestamp = (long long unsigned int)(timeUs * TIME_RES_INV);
}

void DW1000Time::setTime(unsigned long value, float factorUs) {
	float tsValue = value * factorUs;
	tsValue = fmod(tsValue, TIME_OVERFLOW);
	setTime(tsValue);
}

void DW1000Time::setTimestamp(byte data[]) {
	_timestamp = 0;
	for(int i = 0; i < LEN_STAMP; i++) {
	    _timestamp |= ((long long unsigned int)data[i] << (i*8));
	}
}

long long unsigned int DW1000Time::getTimestamp() const {
	return _timestamp;
}

void DW1000Time::getTimestamp(byte data[]) const {
	memset(data, 0, LEN_STAMP);
	for(int i = 0; i < LEN_STAMP; i++) {
	    data[i] = (byte)((_timestamp >> (i*8)) & 0xFF);
	}
}

float DW1000Time::getAsFloat() const {
	return (float)_timestamp * TIME_RES;
}

float DW1000Time::getAsMeters() const {
	return (float)_timestamp * DISTANCE_OF_RADIO;
}

DW1000Time& DW1000Time::operator=(const DW1000Time &assign) {
	if(this == &assign) {
		return *this;
	}
	_timestamp = assign.getTimestamp();
	return *this;
}

DW1000Time& DW1000Time::operator+=(const DW1000Time &add) {
	_timestamp = (_timestamp + add.getTimestamp()) % TIME_OVERFLOW;
	return *this;
}

const DW1000Time DW1000Time::operator+(const DW1000Time &add) const {
    return DW1000Time(*this) += add;
}

DW1000Time& DW1000Time::operator-=(const DW1000Time &sub) {
	long long unsigned int subTS = sub.getTimestamp();
	if(subTS > _timestamp) {
		_timestamp = TIME_OVERFLOW - (subTS - _timestamp);
	} else {
		_timestamp = _timestamp - subTS;
	}
	return *this;
}

const DW1000Time DW1000Time::operator-(const DW1000Time &sub) const {
	return DW1000Time(*this) -= sub;
}

DW1000Time& DW1000Time::operator*=(float factor) {
	float tsValue = (float)_timestamp * factor;
	tsValue = fmod(tsValue, TIME_OVERFLOW);
	_timestamp = (long long unsigned int)tsValue;
	return *this;
}

const DW1000Time DW1000Time::operator*(float factor) const {
	return DW1000Time(*this) *= factor;
}

DW1000Time& DW1000Time::operator/=(float factor) {
	return *this *= (1.0f/factor);
}

const DW1000Time DW1000Time::operator/(float factor) const {
	return DW1000Time(*this) /= factor;
}

boolean DW1000Time::operator==(const DW1000Time &cmp) const {
	return _timestamp == cmp.getTimestamp();
}

boolean DW1000Time::operator!=(const DW1000Time &cmp) const {
	return !(*this == cmp);
}

