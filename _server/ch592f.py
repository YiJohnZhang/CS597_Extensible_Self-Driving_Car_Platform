class CH592FStub:
	"""A minimal stub for the WCH CH592F serial/bridge device.
	This stub buffers written data and returns it on read calls.
	"""

	def __init__(self):
		self._buffer = bytearray()
		self._opened = False

	def open(self):
		self._opened = True

	def write(self, data: bytes) -> int:
		if not self._opened:
			raise RuntimeError("device not opened")
		if not isinstance(data, (bytes, bytearray)):
			data = str(data).encode()
		self._buffer.extend(data)
		return len(data)

	def read(self, n: int = 1024) -> bytes:
		if not self._opened:
			raise RuntimeError("device not opened")
		if not self._buffer:
			return b""
		n = max(0, n)
		out = bytes(self._buffer[:n])
		del self._buffer[:n]
		return out


def create_ch592f():
	return CH592FStub()

