import ConfigParser, os

"""
Device settings:

- Device status,
- Network status,
- Network address,
- Children amount and addresses, <-- The number of children,
- LQI and RSSI, <-- This will be generated automatically,
- Recv node flag,
"""

# Default values.
main_config = { "status": True,
                "nwk_status": True,
                "nwk_addr": 0xdead,
                "children" : 1,
                "recv_status": False }


def config():
    cfg = ConfigParser.RawConfigParser()
    cfg.read("device.cfg");
    try:
        main_config["status"] = cfg.getboolean("device", "status")
        main_config["nwk_status"] = cfg.getboolean("device", "nwk_status")
        main_config["nwk_addr"] = cfg.getint("device", "nwk_addr")
        main_config["children"] = cfg.getint("device", "children")
        # TODO: Add a list of children.
        main_config["recv_status"] = cfg.getboolean("device", "recv_status")
    except Exception as e:
        print "An error occurred in config.py"
        print e
