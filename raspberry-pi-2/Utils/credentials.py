from Crypto.Cipher import AES

class Credentials(object):
    """container to read from file encrypted credentials"""

    # class attribute (vs. instance attribute)
    __aes = AES.new('16 bytes per key', AES.MODE_CBC, '16 byte be IV456')

    def __init__(self, filename):
        if filename != None:
            with open(filename,'r') as f:
                self.__username = f.readline().rstrip(' \n')
                self.__password = f.readline().rstrip(' \n')
                self.__password = self.__aes.decrypt(self.__password).rstrip()
        else:
            self.__username = None
            self.__password = None

    # make user and pwd 'attribute-ish'
    @property
    def user(self):
        return self.__username

    @property
    def pwd(self):
        return self.__password

    def __cypherText(self, password):
        pd = password
        # pad until length is multiple of 16
        while len(pd) % 16 != 0:
            pd = pd + ' '
        return self.__aes.encrypt(pd)

    def generateCredentials(self, username, password):
        self.__username = username
        self.__password = password

        # write username and encrypted password to file
        with open(username+'-credentials.txt','w') as f:
            # python converts '\n' to os.linesep, 
            # so no code changes required for particular os
            f.write(str(username+'\n')) 
            f.write(str(self.__cypherText(password)+'\n'))

