import pathlib,struct,sys
src=pathlib.Path(sys.argv[1]).read_bytes()
if len(src)>1024*1024-16: raise SystemExit('kernel too large')
pathlib.Path(sys.argv[2]).write_bytes(b'TBK1'+bytes(4)+struct.pack('<Q',16)+src)
