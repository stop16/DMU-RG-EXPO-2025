"""
Render the 32x16 dot-matrix font data defined in Merged_Type3.ino to the terminal.

The script parses the `#define CHAR_*` macros and the `const uint16_t ... PROGMEM`
arrays, expands macro references, and converts the 16-bit column values into an
ASCII preview. Each column corresponds to one vertical slice of the display
(most-significant bit -> top row by default).

USAGE EAMPLE

python .\Arduino\Merge\Merged_Type3\render_dotmatrix.py repair_font

OUTPUT (terminal)
repair_font (68 columns)

                #         # ##########  ##########
                #         #          #      ##
                ###########          #      ##
                #         #         #      ####
                #         #         #     ##  ##
                 #########          #    ##    ##

                    ##      ########## ############
                    ##          ##          ##
                ###########     ##          ##
                                ##       ########
                 #########      ##      #        #
                         #      ##      #        #
                         #      ##       ########

"""
from __future__ import annotations

import argparse
import pathlib
import re
import sys
from typing import Dict, Iterable, List, Sequence


DEFAULT_SOURCE = pathlib.Path("Arduino/Merge/Merged_Type3/Merged_Type3.ino")
MACRO_PATTERN = re.compile(r"#define\s+(CHAR_[A-Za-z0-9_]+)\s+([^\n]+)")
ARRAY_PATTERN = re.compile(
    r"const\s+uint16_t\s+([A-Za-z0-9_]+)\s*\[\]\s+PROGMEM\s*=\s*\{(.*?)\}\s*;?",
    re.S,
)


def strip_block_comments(text: str) -> str:
    """Remove C-style block comments so they don't interfere with parsing."""
    return re.sub(r"/\*.*?\*/", "", text, flags=re.S)


def strip_line_comment(fragment: str) -> str:
    """Return the fragment without trailing // comments."""
    idx = fragment.find("//")
    return fragment[:idx] if idx != -1 else fragment


def parse_number(token: str) -> int:
    """Convert a numeric literal (hex/dec/oct/bin) to an integer."""
    token = token.strip()
    base_token = token.lower()
    if base_token.startswith("0x"):
        return int(token, 16)
    if base_token.startswith("0b"):
        return int(token, 2)
    if base_token.startswith("0") and token != "0":
        # Interpret other 0-prefixed values as octal for completeness.
        return int(token, 8)
    return int(token, 10)


def tokenize_initializer_block(block: str) -> List[str]:
    """Split a C-style initializer block into clean tokens."""
    tokens: List[str] = []
    for raw_line in block.splitlines():
        clean_line = strip_line_comment(raw_line).strip()
        if not clean_line:
            continue
        for part in clean_line.split(","):
            token = part.strip()
            if token:
                tokens.append(token)
    return tokens


def parse_font_sources(
    source_path: pathlib.Path,
) -> tuple[Dict[str, List[int]], Dict[str, List[int]]]:
    """Extract CHAR_* macros and font arrays from the source file."""
    try:
        source_text = source_path.read_text(encoding="utf-8")
    except FileNotFoundError as exc:
        raise SystemExit(f"Font source not found: {source_path}") from exc

    source_text = strip_block_comments(source_text)

    macros: Dict[str, List[int]] = {}
    for name, body in MACRO_PATTERN.findall(source_text):
        body = strip_line_comment(body)
        if not body.strip():
            continue
        values = [parse_number(token) for token in body.split(",") if token.strip()]
        macros[name] = values

    arrays: Dict[str, List[int]] = {}
    for name, body in ARRAY_PATTERN.findall(source_text):
        tokens = tokenize_initializer_block(body)
        if not tokens:
            continue

        expanded: List[int] = []
        for token in tokens:
            if token in macros:
                expanded.extend(macros[token])
            else:
                try:
                    expanded.append(parse_number(token))
                except ValueError as exc:  # pragma: no cover - defensive branch
                    raise ValueError(f"Unable to parse token '{token}' in {name}") from exc
        arrays[name] = expanded

    return macros, arrays


def render_columns(
    columns: Sequence[int],
    on_char: str = "#",
    off_char: str = " ",
    lsb_top: bool = False,
) -> List[str]:
    """Convert column data into ASCII rows."""
    if any(val < 0 or val > 0xFFFF for val in columns):
        raise ValueError("Column values must be 16-bit unsigned integers.")

    bit_range: Iterable[int]
    if lsb_top:
        bit_range = range(16)
    else:
        bit_range = range(15, -1, -1)

    rows: List[str] = []
    for bit in bit_range:
        row = "".join(
            on_char if (column >> bit) & 0x1 else off_char for column in columns
        )
        rows.append(row)
    return rows


def main(argv: Sequence[str] | None = None) -> int:
    parser = argparse.ArgumentParser(
        description="Preview the 32x16 dot-matrix font data on the terminal."
    )
    parser.add_argument(
        "names",
        nargs="*",
        help="Macro or font array names to render (e.g. CHAR_TONG disconnected_font).",
    )
    parser.add_argument(
        "--source",
        default=str(DEFAULT_SOURCE),
        help=f"Path to the .ino file containing the font data (default: {DEFAULT_SOURCE})",
    )
    parser.add_argument(
        "--list",
        action="store_true",
        help="List available macro and font names.",
    )
    parser.add_argument(
        "--on",
        default="#",
        help="Character to use for lit pixels (default: '#').",
    )
    parser.add_argument(
        "--off",
        default=" ",
        help="Character to use for unlit pixels (default: space).",
    )
    parser.add_argument(
        "--lsb-top",
        action="store_true",
        help="Interpret the least-significant bit as the top row (default: MSB on top).",
    )

    args = parser.parse_args(argv)

    source_path = pathlib.Path(args.source)
    macros, arrays = parse_font_sources(source_path)

    resources: Dict[str, List[int]] = {}
    resources.update(macros)
    resources.update(arrays)

    if args.list or not args.names:
        max_name = max((len(name) for name in resources), default=0)
        print("Available font resources:")
        for name in sorted(resources):
            columns = len(resources[name])
            print(f"  {name.ljust(max_name)}  ({columns} columns)")
        if not args.names:
            return 0

    exit_code = 0
    for name in args.names:
        sequence = resources.get(name)
        if sequence is None:
            print(f"[!] Unknown font name: {name}", file=sys.stderr)
            exit_code = 1
            continue

        print(f"{name} ({len(sequence)} columns)")
        rendered = render_columns(
            sequence,
            on_char=args.on,
            off_char=args.off,
            lsb_top=args.lsb_top,
        )
        for row in rendered:
            print(row)
        print()  # Spacer between entries.

    return exit_code


if __name__ == "__main__":
    raise SystemExit(main())
