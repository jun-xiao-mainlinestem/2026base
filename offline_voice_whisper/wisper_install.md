# Offline Voice Whisper (Node.js + whisper.cpp)
A minimal Node.js project that:  uses whisper.cpp to transcribe it offline
## Setup

1. Clone whisper.cpp:
```
git clone https://github.com/ggerganov/whisper.cpp
cd whisper.cpp && make && cd ..
```

2. Download model:
```
mkdir -p model
curl -L -o model/ggml-base.en.bin https://huggingface.co/ggerganov/whisper.cpp/resolve/main/ggml-base.en.bin
```

3. Install dependencies:
```
npm install
```

4. Start server:
```
npm start
```
