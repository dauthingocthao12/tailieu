#�^�C�g��
$title = 'Ozzy\'s';

#���Ȃ��̃z�[���y�[�W�ɖ߂邽�߂̂t�q�k
$homepage = 'http://www.ozzys.jp/';

#�Ǘ���
$pass = '1';			#�A�N�Z�X���O������̂Ƀp�X���[�h���K�v���H(Yes=0,No=1)
$pass_key = 'alphatec';		#�p�X���[�h��ݒ肵�����̃p�X���[�h

#IP�`�F�b�N
@IP = ('','');	#�L������IP�͋L�^���Ȃ��B

#�J�E���^�[����@�����l���J�E���g�A�b�v�����Ȃ��B
$coun_up1 = 0;		#���t���ς�閈��OK�I(no = 0 , yes = 1 , ���Ԃ�OK = 2 �A���łȂ����OK = 3)
$count_up2 = 1;		#$coun_up1��2��I�������ꍇ�B���Ԃ��L��

#���̃J�E���^�[��
$cunt_f = '';

#�J�E���^�[�\��
$counth = 0;		#�J�E���^�[�\���@(gif = 0 , text = 1)
$countk = 5;		#�J�E���^�[�\����

#GIF�J�E���^�[�摜�̒u���Ă���ꏊ�i�t���p�X�Łj
$cun_image = 'http://www.ozzys.jp/count/image/count'; #�t���p�X��(SSI�p)
$cun_image2 = './image/count'; #����cgi�̃t�@�C���̏ꏊ����B
#�N�b�L�[���i�[���閼�O��ݒ肷��
$CookieName = 'alpha';

#SSI�𗘗p���邩�H
$ssi = 0;	#yes = 0, no = 1

#���b�N
$lockkey  = 2;			# �t�@�C�����b�N�`�� (0=no 1=symlink�֐� 2=open�֐�)

#cgi�t�@�C��
$script  = 'look.cgi';		#�{���pcgi
$script2 = 'access.cgi';	#�f�[�^�[�L�^cgi
$script3 = 'pass.cgi';		#�p�X���[�h�ݒ�cgi

$salt = 'al';

#�f�[�^�[�������Ă����̌���������邩�H
$ne = 0;	#yes:0,no:1

#�X�̏ڍ׃f�[�^�[�̍ŏ��̕\��(��\�� 0,�\�� 1)
$a_1 = 1;	#�����N��
$a_2 = 1;	#�L�[���[�h
$a_3 = 0;	#OS���
$a_4 = 0;	#�u���E�U�[���
$a_5 = 0;	#�v���o�C�_�[
$a_6 = 0;	#�v���L�V

#�f�[�^�[�t�@�C��
$logfile_a = "./log/count.dat";			#�J�E���^�[����

$bgcolor="#ffffff";

$hk = 50;	#���\����

@MON = ('0','31','28','31','30','31','30','31','31','30','31','30','31');
@WEEK = ('��','��','��','��','��','��','�y');

#�Փ��ݒ�
sub holyday {
		if (($m == 1) && ($d1 == 1)){ $flags = 1; }
		if (($m == 2) && ($d1 == 11)){ $flags = 1; }
		if (($m == 4) && ($d1 == 29)){ $flags = 1; }
		if (($m == 5) && ($d1 == 3)){ $flags = 1; }
		if (($m == 5) && ($d1 == 4)){ $flags = 1; }
		if (($m == 5) && ($d1 == 5)){ $flags = 1; }
		if (($m == 7) && ($d1 == 20)){ $flags = 1; }
		if (($m == 9) && ($d1 == 15)){ $flags = 1; }
		if (($m == 11) && ($d1 == 3)){ $flags = 1; }
		if (($m == 11) && ($d1 == 23)){ $flags = 1; }
		if (($m == 12) && ($d1 == 23)){ $flags = 1; }


		$idou3 = (($y - 2000) * 0.242194);
		$uru3 = int(($y - 2000) / 4);
		$hol3 = int(20.69115 + $idou3 - $uru3);
		if (($m == 3) && ($d1 == $hol3)){ $flags = 1; }

		$idou9 = (($y - 2000) * 0.242194);
		$uru9 = int(($y - 2000) / 4);
		$hol9 = int(23.09 + $idou9 - $uru9);
		if (($m == 9) && ($d1 == $hol9)){ $flags = 1; }

		if (($m == 1) && ($d1 >= 8) && ($d1 <= 14) && ($amari == 2)){ $flags = 1; }
		if (($m == 10) && ($d1 >= 8) && ($d1 <= 14) && ($amari == 2)){ $flags = 1; }

}

1;
