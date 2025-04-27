import sys
import logging
from pymodbus.server import StartSerialServer
from pymodbus.device import ModbusDeviceIdentification
from pymodbus.datastore import ModbusSlaveContext, ModbusServerContext
from pymodbus.datastore import ModbusSequentialDataBlock

if len(sys.argv) < 2:
    print("Uso: python3 modbus_slave.py /dev/pts/X")
    sys.exit(1)

device = sys.argv[1]

logging.basicConfig()
log = logging.getLogger()
log.setLevel(logging.DEBUG)

# Registradores simulados
store = ModbusSlaveContext(
    hr=ModbusSequentialDataBlock(0, [0]*100)
)
context = ModbusServerContext(slaves=store, single=True)

identity = ModbusDeviceIdentification()
identity.VendorName = 'SimulatedDevice'
identity.ProductCode = 'SD'
identity.VendorUrl = 'http://example.com'
identity.ProductName = 'RoomAccessControlSim'
identity.ModelName = 'SimModel'
identity.MajorMinorRevision = '1.0'

StartSerialServer(context, identity=identity,
                  port=device,
                  baudrate=9600, parity='N', stopbits=1, bytesize=8)
