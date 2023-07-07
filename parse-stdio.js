const header_root = __dirname;

function readHeaderFile(file) {
	const fs = require('fs');
	const path = require('path');
	return fs.readFileSync(path.join(header_root, file)).toString();
}


const chunks = [];

process.stdin.on('data', (chunk) => {
	chunks.push(chunk);
});

process.stdin.on('end', () => {
	const input = Buffer.concat(chunks).toString('utf8');
	console.log(parse(input, readHeaderFile));
});