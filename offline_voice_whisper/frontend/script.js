const recordBtn = document.getElementById("recordBtn");
const output = document.getElementById("output");

let mediaRecorder;
let audioChunks = [];

recordBtn.addEventListener("click", async () => {
  const stream = await navigator.mediaDevices.getUserMedia({ audio: true });
  mediaRecorder = new MediaRecorder(stream);

  audioChunks = [];

  mediaRecorder.ondataavailable = event => {
    audioChunks.push(event.data);
  };

  mediaRecorder.onstop = async () => {
    const blob = new Blob(audioChunks, { type: 'audio/webm' });
    const formData = new FormData();
    formData.append('audio', blob, 'recording.webm');

    try {
      const res = await fetch('/upload', {
        method: 'POST',
        body: formData
      });

      const contentType = res.headers.get("content-type");
      if (res.ok && contentType && contentType.includes("application/json")) {
        const data = await res.json();
        output.textContent = `🗣️ Transcription: ${data.transcript}`;
      } else {
        const errorText = await res.text();
        output.textContent = `❌ Server error: ${errorText}`;
      }
    } catch (err) {
      output.textContent = `❌ Fetch error: ${err.message}`;
    }
  };

  mediaRecorder.start();
  recordBtn.disabled = true;

  setTimeout(() => {
    mediaRecorder.stop();
    recordBtn.disabled = false;
  }, 5000); // record for 5 seconds
});
