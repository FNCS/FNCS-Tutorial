FNCS2-Tutorial
==============
Welcome to the Framework for Network Co-Simulation 2 (FNCS2) Tutorial.
This tutorial features four step-by-step guides that target the basics
('two-simulators'), a modestly complex transactive control
application ('GridLAB-D+ns-3'), and a fully-featured transactive control
application with multiple feeders, retail markets, and a wholesale
market inside a transmission-level optimal power flow ('large').

The [two-simulators demo](demo-two-simulators) showcases the
fundamentals of running co-simulations using the FNCS framework. We will
walk you through installing FNCS and its dependencies, CZMQ and ZeroMQ,
as well as how to run two sample simulators within the co-simulation
platform provided by FNCS. The code samples provided will illustrate the
principles of co-simulation including message exchange and clock
synchronization.

The [GridLAB-D+ns-3 demo](demo-gld-ns3) will walk you through installing
FNCS, GridLAB-D, ns-3, and all prerequisite software. This complex
example application demonstrates a real use case of transactive control,
exchanging market prices and bids through a simulated network. A single
GridLAB-D instance simulates a single feeder and market, where the bids
and price signals are routed through the communication network modeled
by ns-3.

The [large demo](demo-large) will walk you through installing FNCS,
GridLAB-D, ns-3, MATPOWER, and all prerequisite software. This complex
example application demonstrates a real use case of transactive control,
exchanging market prices and bids through a simulated network. Multiple
GridLAB-D instances individually simulate a single feeder and market,
while the cumulative demand of each feeder is exchanged with a MATPOWER
optimal power flow solver at the tranmission level.

We hope you find these demonstration cases useful. Please use the
GitHub Issues to notify us of any problems with the tutorial text.
