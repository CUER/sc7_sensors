# To build proto files

## Pre-built install

- Add nanopb/generator-bin to path

```bash
protoc --python_out=pyi_out:. simple.proto
protoc --nanopb_out=. simple.proto
cp nanopb/*.c nanopb/*.h .
```

## Python install

```bash
pip install protobuf grpcio-tools nanopb
python -m grpc_tools.protoc -I. --python_out=pyi_out:. simple.proto
python -m grpc_tools.protoc -I. --plugin=protoc-gen-nanopb.exe --nanopb_out=. simple.proto
```
