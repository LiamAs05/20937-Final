from Crypto.PublicKey import RSA
from Crypto.Cipher import PKCS1_OAEP


def encrypt_key(public_key_bytes: bytes, aes_key: bytes):
    key = RSA.import_key(public_key_bytes)
    cipher = PKCS1_OAEP.new(key)
    return cipher.encrypt(aes_key)
