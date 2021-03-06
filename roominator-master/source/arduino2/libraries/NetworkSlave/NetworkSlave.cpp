#include "NetworkSlave.h"
#include "WProgram.h"
#include "Wire.h"

NetworkSlave::NetworkSlave() {
  ud = UpstreamData();
  ud.setCancel(false);
  ud.setReserve(0);

  dd = DownstreamData();
}

int NetworkSlave::getCancel() {
  return ud.getCancel();
}

int NetworkSlave::getReserve() {
  return ud.getReserve();
}

void NetworkSlave::setDownstreamData(char* received) {
  if (getCancel() || getReserve()) {
    Serial.println("skipping this message from the master");
    return;
  }

  dd.parseAndUpdate(received);
}

void NetworkSlave::reserve() {
  ud.setCancel(false);
  ud.setReserve(ud.getReserve()+1);
}

void NetworkSlave::cancel() {
  if (dd.getCurrentReservation()) {
    ud.setReserve(0);
    ud.setCancel(true);
  }
}

DownstreamDataStruct* NetworkSlave::getDownstreamData() {
  return dd.getCurrentReservation();
}

void NetworkSlave::clearCounts() {
  ud.setReserve(0);
  ud.setCancel(false);
}
