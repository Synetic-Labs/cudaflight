/*
 * This file is part of Cleanflight and Betaflight.
 *
 * Cleanflight and Betaflight are free software. You can redistribute
 * this software and/or modify this software under the terms of the
 * GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version.
 *
 * Cleanflight and Betaflight are distributed in the hope that they
 * will be useful, but WITHOUT ANY WARRANTY; without even the implied
 * warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this software.
 *
 * If not, see <http://www.gnu.org/licenses/>.
 */

// Sink serial backend for the SITL_LOCKSTEP target, replacing the TCP
// (dyad) backend of stock SITL. N firmware instances in one process
// cannot each bind the same TCP ports, and the GPU target has no
// sockets at all; MSP/CLI access is not part of the lockstep use case.
// Writes are swallowed, reads return nothing.

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "platform.h"

#include "drivers/serial.h"
#include "drivers/serial_tcp.h"

static serialPort_t sinkPorts[SERIAL_PORT_COUNT];
static unsigned sinkPortsUsed = 0;

static void sinkWrite(serialPort_t *instance, uint8_t ch)
{
    UNUSED(instance);
    UNUSED(ch);
}

static uint32_t sinkRxWaiting(const serialPort_t *instance)
{
    UNUSED(instance);
    return 0;
}

static uint32_t sinkTxFree(const serialPort_t *instance)
{
    UNUSED(instance);
    return UINT32_MAX;
}

static uint8_t sinkRead(serialPort_t *instance)
{
    UNUSED(instance);
    return 0;
}

static void sinkSetBaudRate(serialPort_t *instance, uint32_t baudRate)
{
    instance->baudRate = baudRate;
}

static bool sinkTxBufferEmpty(const serialPort_t *instance)
{
    UNUSED(instance);
    return true;
}

static void sinkSetMode(serialPort_t *instance, portMode_e mode)
{
    instance->mode = mode;
}

static void sinkSetCtrlLineStateCb(serialPort_t *instance, void (*cb)(void *context, uint16_t ctrlLineState), void *context)
{
    UNUSED(instance);
    UNUSED(cb);
    UNUSED(context);
}

static void sinkSetBaudRateCb(serialPort_t *instance, void (*cb)(serialPort_t *context, uint32_t baud), serialPort_t *context)
{
    UNUSED(instance);
    UNUSED(cb);
    UNUSED(context);
}

static void sinkWriteBuf(serialPort_t *instance, const void *data, int count)
{
    UNUSED(instance);
    UNUSED(data);
    UNUSED(count);
}

static void sinkBeginWrite(serialPort_t *instance)
{
    UNUSED(instance);
}

static void sinkEndWrite(serialPort_t *instance)
{
    UNUSED(instance);
}

static const struct serialPortVTable sinkVTable = {
    .serialWrite = sinkWrite,
    .serialTotalRxWaiting = sinkRxWaiting,
    .serialTotalTxFree = sinkTxFree,
    .serialRead = sinkRead,
    .serialSetBaudRate = sinkSetBaudRate,
    .isSerialTransmitBufferEmpty = sinkTxBufferEmpty,
    .setMode = sinkSetMode,
    .setCtrlLineStateCb = sinkSetCtrlLineStateCb,
    .setBaudRateCb = sinkSetBaudRateCb,
    .writeBuf = sinkWriteBuf,
    .beginWrite = sinkBeginWrite,
    .endWrite = sinkEndWrite,
};

serialPort_t *serTcpOpen(serialPortIdentifier_e identifier, serialReceiveCallbackPtr rxCallback, void *rxCallbackData, uint32_t baudRate, portMode_e mode, portOptions_e options)
{
    if (sinkPortsUsed >= SERIAL_PORT_COUNT) {
        return NULL;
    }

    serialPort_t *port = &sinkPorts[sinkPortsUsed++];
    memset(port, 0, sizeof(*port));
    port->vTable = &sinkVTable;
    port->identifier = identifier;
    port->rxCallback = rxCallback;
    port->rxCallbackData = rxCallbackData;
    port->baudRate = baudRate;
    port->mode = mode;
    port->options = options;

    return port;
}
