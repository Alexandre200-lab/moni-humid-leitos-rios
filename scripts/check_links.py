#!/usr/bin/env python3
"""Valida links locais em arquivos Markdown (sem rede, deterministico)."""

from __future__ import annotations

import os
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
LINK_RE = re.compile(r"\[[^\]]*\]\(([^)]+)\)")
SKIP_PREFIXES = ("http://", "https://", "mailto:", "#", "/")


def main() -> int:
    errors: list[str] = []
    checked = 0

    for path in sorted(ROOT.rglob("*.md")):
        if ".git" in path.parts:
            continue
        text = path.read_text(encoding="utf-8")
        for target in LINK_RE.findall(text):
            target = target.strip().split()[0] if target.strip() else ""
            if not target or target.startswith(SKIP_PREFIXES):
                continue
            relative = target.split("#", 1)[0].split("?", 1)[0]
            if not relative:
                continue
            resolved = (path.parent / relative).resolve()
            checked += 1
            if not resolved.exists():
                errors.append(f"{path.relative_to(ROOT)}: link quebrado -> {target}")

    if errors:
        print("\n".join(errors), file=sys.stderr)
        print(f"{len(errors)} link(s) quebrado(s).", file=sys.stderr)
        return 1

    print(f"OK: {checked} links locais verificados.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
