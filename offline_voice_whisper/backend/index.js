import express from 'express';
import multer from 'multer';
import { exec } from 'child_process';
import fs from 'fs';
import path from 'path';
import { fileURLToPath } from 'url';

const __dirname = path.dirname(fileURLToPath(import.meta.url));
const app = express();
const upload = multer({ dest: 'uploads/' });

app.use(express.static(path.join(__dirname, '../frontend')));
app.post('/upload', upload.single('audio'), (req, res) => {
  const inputPath = req.file.path;
  const outputDir = 'transcriptions';
  const model = 'tiny'; // You can use tiny, base, small, etc.

  if (!fs.existsSync(outputDir)) {
    fs.mkdirSync(outputDir);
  }

  const cmd = `whisper ${inputPath} --language en --model ${model} --output_dir ${outputDir} --output_format txt`;

  exec(cmd, (err, stdout, stderr) => {
    if (err) {
      console.error('Whisper error:', stderr);
      res.status(500).send('Transcription failed');
      return;
    }

    // Whisper outputs a file like uploads/abc123.txt in the output_dir
    const outputFile = path.join(outputDir, path.basename(inputPath) + '.txt');

    fs.readFile(outputFile, 'utf8', (err, data) => {
      if (err) {
        console.error('File read error:', err);
        res.status(500).send('Could not read transcript');
        return;
      }

      res.send({ transcript: data });
    });
  });
});

app.listen(3000, () => console.log('Server running on http://localhost:3000'));
