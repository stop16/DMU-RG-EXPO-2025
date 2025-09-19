def reverse_bits(value, num_bits=16):
    """지정된 값의 비트 순서를 뒤집어 새로운 정수 값을 반환합니다."""
    # 숫자를 2진수 문자열로 변환하고, 지정된 비트 수만큼 0으로 채웁니다.
    binary_string = bin(value)[2:].zfill(num_bits)
    # 2진수 문자열을 뒤집고, 다시 정수로 변환합니다.
    return int(binary_string[::-1], 2)

# 비트를 뒤집을 16진수 값들의 리스트입니다.
hex_list = [
  0x0000, 0x0000, 0x0100, 0x0180, 0x00c0, 0x0060, 0x7c3e, 0x4060, 0x40c0, 0x4180, 0x4100, 0x4000, 0x41fe, 0x0000, 0x0000, 0x0000
]

# 리스트의 각 항목에 대해 reverse_bits 함수를 실행합니다.
reversed_list = [reverse_bits(item) for item in hex_list]

# 결과를 네 자리 16진수 문자열 형식으로 출력합니다.
formatted_output = [f'0x{val:04x}' for val in reversed_list]

print(', '.join(formatted_output))