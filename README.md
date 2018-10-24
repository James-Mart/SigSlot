# SigSlot
This project is a lean, fast, modern, typesafe, minimum viable reimplementation of the signal/slot pattern introduced by Qt.

# What is it?
The signal/slot pattern is essentially a glorified observer pattern, allowing observers to receive messages from subjects, without requiring the subject to know about its observers. Signals and slots take that simple pattern several steps further, allowing subjects to provide interfaces for other objects to not only observe, but modify the subject's behavior.

For example, someone could create a library of UI controls using a signal interface to allow any interested "observer" to watch the UI control for events, filter out illegal values, or respond to arbitrary queries.
