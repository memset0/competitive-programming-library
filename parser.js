const header_root = __dirname;
const line_limit = 120;
const copyrights = `
/**
 *    author:   memset0
 *    date:     ${Date()}
 *    blog:     https://mem.ac/
 *    library:  https://github.com/memset0/competitive-programming-library
**/
`.trim();
const endl = '\n';
const chunks = [];

function _readHeaderFile(file) {
	const fs = require('fs');
	const path = require('path');
	return fs.readFileSync(path.join(header_root, file)).toString();
}

String.prototype.replace = function (find, replace) {
	return this.replace(new RegExp(find.replace(/[.*+?^${}()|[\]\\]/g, '\\$&'), 'g'), replace);
}

String.prototype.isSpace = function () {
	return this == ' ' || this == '\t' || this == '\n' || this == '\r' || this == '\v' || this == '\f';
}

String.prototype.isIdentifierPart = function () {
	return ('a' <= this && this <= 'z') || ('A' <= this && this <= 'Z') || ('0' <= this && this <= '9') || this == '$' || this == '_';
}

String.prototype.cut = function (lef, rig, ind) {
	return this.substring(lef[ind], rig[ind] + 1);
}

String.prototype.safeIndexOf = function (ch, ind) {
	while (ind != this.length && !(this[ind].isSpace())) {
		if (this[ind] == ch) return ind;
		++ind;
	}
	return -1;
}

process.stdin.on('data', (chunk) => {
	chunks.push(chunk);
});

process.stdin.on('end', () => {
	const input = Buffer.concat(chunks).toString('utf8');
	console.log(parse(input, _readHeaderFile));
});

function parse(source, readHeaderFile) {
	let headers = '';
	let body = '';

	const included = {};

	function $compress(str) {
		const patternS = "R\"(";
		const patternT = ")\"";
		const trigraphs = { '=': '#', '/': '\\', '\'': '^', '(': '[', ')': ']', '<': '{', '>': '}', '!': '|', '-': '~' };
		function check(a, b) { return (a.isIdentifierPart() && b.isIdentifierPart()) || ((a == ' + ' || a == '-') && a == b) || (a == '/' && b == '*'); }

		function processTrigraph(str) {
			for (const ch in trigraphs) {
				let ret = '';
				let lst = 0;
				let pattern = "??" + ch;
				while (lst !== str.length) {
					const ind = str.indexOf(pattern, lst);
					if (ind === -1) break;
					ret += str.substring(lst, ind) + trigraphs[ch];
					lst = ind + pattern.length;
				}
				ret += str.substring(lst);
				str = ret;
			}
			return str;
		}

		function processLineBreak(str) {
			let lines = str.split(endl);
			let ret = '';
			for (let i = 0; i < lines.length; ++i) {
				let line = lines[i];
				let j = line.length;
				while ((--j) >= 0 && line[j].isSpace());
				if (line[j] == '\\') ret += line.substring(0, j);
				else ret += line + endl;
			}
			if (ret.length !== 0 && ret[ret.length - 1] == endl)
				ret = ret.substring(0, ret.length - 1);
			return ret;
		}

		function processMultilineCommentAndReplaceMark(str) {
			let ret = '';
			let stringBegin, esc = false;
			let inString = false;
			let lst = 0;
			for (let i = 0; i < str.length; ++i) {
				if (inString) {
					if ((!esc) && str[i] == stringBegin) inString = false;
				} else if (str[i] == '\'' || str[i] == '"') {
					inString = true;
					stringBegin = str[i];
				} else if (i + 1 < str.length) {
					if (str[i] == '/' && str[i + 1] == '*') {
						ret += str.substring(lst, i) + ' ';
						i = str.indexOf('*/', i + 2) + 2;
						lst = i;
						if (i == -1) break;
					} else if (str[i] == '%' && str[i + 1] == ':') {
						ret += str.substring(lst, i) + "#";
						lst = i += 2;
					}
				}
				esc = str[i] == '\\';
			}
			ret += str.substring(lst);
			return ret;
		}

		function compressSingle(str) {
			str = processTrigraph(str);
			str = processLineBreak(str);
			str = processMultilineCommentAndReplaceMark(str);

			let arr = str.split(endl);
			let ret = "";
			let forceNewline = true;
			let last = '\0';
			for (let w = 0; w < arr.length; ++w) {
				let line = arr[w];
				if (line.length === 0) continue;
				let i = 0;
				let lef = [];
				let rig = [];
				lex: while (true) {
					while (i != line.length && line[i].isSpace()) ++i;
					if (i === line.length) break;
					lef.push(i);
					let stringBegin, esc = false;
					let inString = false;
					while (i != line.length && (inString || (!(line[i].isSpace())))) {
						if (inString) {
							if ((!esc) && line[i] == stringBegin) inString = false;
						} else if (line[i] == '"' || line[i] == '\'') {
							inString = true;
							stringBegin = line[i];
						} else if (line[i] == '/' && i != line.length - 1) {
							if (line[i + 1] == '/') {
								rig.push(i - 1);
								lef.push(i);
								rig.push(line.length - 1);
								break lex;
							}
						}
						++i;
						if (i == line.length) break;
						esc = line[i] == '\\';
					}
					rig.push(i - 1);
				}
				let n = lef.length;
				if (!n) continue;
				let originForceNewline = forceNewline;
				let originLast = last;
				if (last != '\0') {
					if (forceNewline) ret += endl;
					else if (check(last, line[lef[0]])) ret += ' ';
				}
				last = line[rig[n - 1]];
				let j = 0;
				if (line[lef[0]] == '#') {
					if (!forceNewline) ret += endl;
					forceNewline = true;
					if (!line[lef[0] + 1]) line[--lef[j = 1]] = '#';
					ret += line.cut(lef, rig, j);
					if (line[lef[j] + 1] == 'd' && line.safeIndexOf('(', lef[j + 1]) === -1) {
						ret += ' ' + line.cut(lef, rig, ++j);
						if (j + 1 < n) ret += ' ' + line.cut(lef, rig, ++j);
					}
					++j;
				} else forceNewline = false;
				for (; j < n; ret += line.cut(lef, rig, j++)) {
					if (lef[j] + 2 <= line.length) {
						if (line.substring(lef[j], lef[j] + 2) == '//') {
							if (j == 0) forceNewline = originForceNewline;
							break;
						}
					}
					if (j && check(line[rig[j - 1]], line[lef[j]])) ret += ' ';
				}
				last = j ? line[rig[j - 1]] : originLast;
			}
			return ret;
		}

		let ret = '';
		let lst = 0;
		while (lst !== str.length) {
			let ind = str.indexOf(patternS, lst);
			if (ind === -1) break;
			ret += compressSingle(str.substring(lst, ind));
			let rig = str.indexOf(patternT, ind + patternS.length);
			if (rig === -1) {
				ret += str.substring(ind);
				lst = str.length;
				break;
			}
			lst = rig + patternT.length;
			ret += str.substring(ind, lst);
		}
		ret += compressSingle(str.substring(lst));
		return ret;
	}

	function compress(str, file) {
		str = $compress(str);
		if (included['mem/global.hpp'] && file != 'mem/global.hpp') {
			str = str.replaceAll('unsigned long long', ' ull ');
			str = str.replaceAll('long long', ' ll ');
			str = str.replaceAll('unsigned int', ' uint ');
			str = str.replaceAll('long double', ' llf ');
			str = str.replaceAll('double', ' lf ');
			str = str.replaceAll('inline', ' IL ');
			str = str.replaceAll('operator', ' OP ');
			str = str.replaceAll('template<typename T>', ' TT ');
		}
		return $compress(str);
	}

	function parseHeader(file) {
		file = file.replaceAll('\\', '/');
		if (included[file]) {
			return;
		}
		included[file] = true;

		const source = readHeaderFile(file);
		const program = [''];
		let still_in_define = false;
		for (let line of source.split('\n')) {
			if (line.startsWith('#include')) {
				const name = line.slice(8).trim();
				if ((name.startsWith('"') && name.endsWith('"')) || (name.startsWith('<mem/') && name.endsWith('>'))) {
					const filename = name.slice(1, -1);
					const path = file.split('/');
					path.pop();
					while (filename.startsWith('.')) {
						if (!filename.includes('/')) { break; }
						let k = filename.match('/').index;
						while (k && filename[k - 1] == '.') --k;
						if (!k) {
							for (let _ = filename.match('/').index - 1; path.length && _--;) { path.pop(); }
						} else {
							break;
						}
						filename = filename.slice(filename.match('/').index + 1);
					}
					path.push(filename);
					parseHeader(path.join('/'));
				}
			} else if (line.startsWith('#') || still_in_define) {
				const line_filtered = line.endsWith('\\') ? line.slice(0, line.length - 1) : line;
				if (still_in_define) {
					program[program.length - 1] += line_filtered;
				} else {
					if (!program[program.length - 1]) {
						program[program.length - 1] = line_filtered;
					} else {
						program.push(line_filtered);
					}
				}
				if (line.endsWith('\\')) {
					still_in_define = true;
				} else {
					still_in_define = false;
					program.push('');
				}
			} else {
				const match_comment = line.match('//');
				if (match_comment) {
					line = line.slice(0, match_comment.index);
				}
				if (line.endsWith('\\')) {
					line = line.slice(0, line.length - 1);
				}
				line = line.trim();
				if (line) {
					program[program.length - 1] += line.trim() + ' ';
				}
			}
		}

		if (!program[program.length - 1]) {
			program.pop();
		}
		for (let i = 0; i < program.length; i++) {
			const code = compress(program[i], file);
			if (code.length > line_limit) {
				program[i] = '';
				for (let j = 0; true; j += line_limit - 1) {
					if (code.length - j <= line_limit) {
						program[i] += code.slice(j, code.length);
						break;
					} else {
						program[i] += code.slice(j, j + line_limit - 1) + '\\' + endl;
					}
				}
			} else {
				program[i] = code;
			}
		}

		if (program[0].startsWith('#ifndef') && program[1].startsWith('#define') && program[program.length - 1].startsWith('#endif')) {
			// qualified header
			program.shift();
			program.shift();
			program.pop();
		}

		// console.error(program);
		if (program.length) {
			headers += '// ' + file + endl + program.join(endl) + endl;
		}
	}

	headers += '#include<bits/stdc++.h>' + endl;
	for (const line of source.split('\n')) {
		if (line.startsWith('#include')) {
			const name = line.slice(8).trim();
			if ((name.startsWith('"') && name.endsWith('"')) || (name.startsWith('<mem/') && name.endsWith('>'))) {
				parseHeader(name.slice(1, -1));
			}
		} else {
			body += line + endl;
		}
	}

	return copyrights.replaceAll('\n', endl) + endl + headers + body;
}