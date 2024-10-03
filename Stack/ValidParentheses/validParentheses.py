def isValid(s):
    stack = []
    for c in s:
        if c in '{[(':
            stack.append(c)
        else:
            if not stack or \
                (c == ')' and stack[-1] != '(') or \
                (c == ']' and stack[-1] != '[') or \
                (c == '}' and stack[-1] != '{'):
                return False
            stack.pop()
    return not stack

def isValid1(s):
    stack = []
    mapping = {")":"(", "}":"{", "]":"["}

    for c in s:
        if c in mapping.values():
            stack.append(c)
        elif c in mapping.keys():
            if not stack or mapping[c] != stack.pop():
                return False

    return not stack

def main():
    print(f"{isValid1('[()]{()}')}")

if __name__ == "__main__":
    main()