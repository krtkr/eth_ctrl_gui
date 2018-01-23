# eth_ctrl_gui
Basic test GUI for EthCtrl series of my boards.

Current version uses raw posix sockets to communicate with the board.

It consist of:

1. simple c library (eth_ctrl.\*)
2. Pieces of a simple modular execution engine
3. GUI itself

# Build

Can be easily build using QT5 under linux.

# Execution without root

To allow application open raw socket without root one can use following command:

```sh
sudo -S setcap 'CAP_NET_RAW+eip CAP_NET_ADMIN+eip' eth_ctrl_gui
```

