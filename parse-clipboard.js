const fs = require('fs');
const path = require('path');
const clipboardy = require('clipboardy');

const parse = require('./parser');
const header_root = __dirname;

function readHeaderFile(file) {
  return fs.readFileSync(path.join(header_root, file)).toString();
}

async function parseClipboard() {
  const code = await clipboardy.read();
  console.log(code);
  const parsed = parse(code, readHeaderFile);
  console.log(parsed);
  clipboardy.write(parsed);
}

parseClipboard();