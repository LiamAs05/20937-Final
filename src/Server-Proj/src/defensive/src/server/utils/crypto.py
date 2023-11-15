from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP, AES


def encrypt_key(public_key_bytes: bytes, aes_key: bytes):
    key = RSA.import_key(public_key_bytes)
    cipher = PKCS1_OAEP.new(key)
    return cipher.encrypt(aes_key)

def decrypt_aes(aes_key: bytes):
    cipher = AES.new(aes_key, AES.MODE_CBC)
    data = cipher.decrypt(cipher)
    return data