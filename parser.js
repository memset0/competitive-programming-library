const fs = require('fs');
const path = require('path');

const header_root = __dirname;
const chunks = [];

process.stdin.on('data', (chunk) => {
	chunks.push(chunk);
});

process.stdin.on('end', () => {
	const input = Buffer.concat(chunks).toString('utf8');
	console.log(parse(input));
});

function parse(source) {
	let copyrights = '';
	let headers = '';
	let body = '';

	const included = {};

	function parseHeader(file) {
		if (included[file]) {
			return;
		}
		included[file] = true;

		const source = fs.readFileSync(path.join(header_root, file)).toString();
		const program = [''];
		let still_in_define = false;
		for (let line of source.split('\n')) {
			if (line.startsWith('#include')) {
				const name = line.slice(8).trim();
				if ((name.startsWith('"') && name.endsWith('"')) || (name.startsWith('<mem/') && name.endsWith('>'))) {
					const filename = name.slice(1, -1);
					parseHeader(path.join(path.dirname(file), filename));
				}
			} else if (line.startsWith('#') || still_in_define) {
				if (!program[program.length - 1]) {
					program[program.length - 1] = line;
				} else {
					program.push(line);
				}
				program.push('');
				if (line.endsWith('\\')) {
					still_in_define = true;
				} else {
					still_in_define = false;
				}
			} else {
				const match_comment = line.match('//');
				if (match_comment) {
					line = line.slice(0, match_comment.index);
				}
				line = line.trim()
				if (line) {
					program[program.length - 1] += line.trim() + ' ';
				}
			}
		}

		if (!program[program.length - 1]) {
			program.pop();
		}
		console.error(program);
		headers += program.join('\n') + '\n\n';
	}

	headers += '#include<bits/stdc++.h>\n\n';
	for (const line of source.split('\n')) {
		if (line.startsWith('#include')) {
			const name = line.slice(8).trim();
			if ((name.startsWith('"') && name.endsWith('"')) || (name.startsWith('<mem/') && name.endsWith('>'))) {
				parseHeader(path.join('.', name.slice(1, -1)));
			}
		} else {
			body += line + '\n';
		}
	}

	return copyrights + headers + body;
}