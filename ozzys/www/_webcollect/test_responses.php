<!doctype html>
<html lang="ja">
	<head>
		<title>=^._.^=</title>

		<meta http-equiv="Content-Type" content="text/html; charset=Shift_JIS" />
		<meta name="viewport" content="initial-scale=1.0,width=device-width" />

		<link rel="stylesheet" href="//fonts.googleapis.com/css?family=Roboto:300,300italic,700,700italic">
		<link rel="stylesheet" href="//cdn.rawgit.com/necolas/normalize.css/master/normalize.css">
		<link rel="stylesheet" href="//cdn.rawgit.com/milligram/milligram/master/dist/milligram.min.css">

		<script src="https://code.jquery.com/jquery-3.3.1.slim.min.js"></script>

        <style>
            .kuroneko {
                color: white;
                background: black;
                border-radius: 5px;
                padding: 3px 5px;
            }
        </style>
	</head>
	<body class="container">

		<hr>
		<center><b>注文データ（POST）</b></center>
		<hr>

		<table>
			<tr>
				<th>TRS_MAP</th>
				<td><?= $_POST['TRS_MAP'] ?></td>
			</tr>
			<tr>
				<th>trader_code</th>
				<td><?= $_POST['trader_code'] ?></td>
			</tr>
			<tr>
				<th>order_no</th>
				<td><?= $_POST['order_no'] ?></td>
			</tr>
			<tr>
				<th>goods_name</th>
				<td><?= $_POST['goods_name'] ?></td>
			</tr>
			<tr>
				<th>settle_price</th>
				<td><?= $_POST['settle_price'] ?></td>
			</tr>
			<tr>
				<th>buyer_name_kanji</th>
				<td><?= $_POST['buyer_name_kanji'] ?></td>
			</tr>
			<tr>
				<th>buyer_tel</th>
				<td><?= $_POST['buyer_tel'] ?></td>
			</tr>
			<tr>
				<th>buyer_email</th>
				<td><?= $_POST['buyer_email'] ?></td>
			</tr>
			<tr>
				<th>buyer_name_kana</th>
				<td><?= $_POST['buyer_name_kana'] ?></td>
			</tr>
			<tr>
				<th>return_url</th>
				<td><?= $_POST['return_url'] ?></td>
			</tr>
			<tr>
				<th>payment_method</th>
				<td><?= $_POST['payment_method'] ?></td>
			</tr>
			<tr>
				<th>success_url</th>
				<td><?= $_POST['success_url'] ?></td>
			</tr>
			<tr>
				<th>failure_url</th>
				<td><?= $_POST['failure_url'] ?></td>
			</tr>
			<tr>
				<th>cancel_url</th>
				<td><?= $_POST['cancel_url'] ?></td>
			</tr>
		</table>

		<hr>
		<center><b>シミュレーション</b></center>
		<hr>

		<div class="row">
			<div class="column">
				<h2>OK</h2>
				<form action="<?= $_POST['return_url'] ?>" method="POST" target="_blank">
					<input type="hidden" name="trader_code"   value="<?= $_POST['trader_code'] ?>" />
					<input type="hidden" name="order_no"      value="<?= $_POST['order_no'] ?>" />
					<input type="hidden" name="settle_price"  value="12345" />
					<input type="hidden" name="settle_date"   value="<?= date('YmdHis') ?>" />
					<input type="hidden" name="settle_result" value="1" />
					<input type="hidden" name="settle_detail" value="4" />
					<input type="hidden" name="settle_method" value="9" /> <!-- VISA -->
					<button type="submit">自動リクエストをSend</button>
				</form>

				<a href="<?= $_POST['success_url'] ?>" class="button">ユーザが戻る</a>
			</div>

			<div class="column">
				<h2>FAILURE</h2>
				<form action="<?= $_POST['return_url'] ?>" method="POST" target="_blank">
					<input type="hidden" name="trader_code"   value="<?= $_POST['trader_code'] ?>" />
					<input type="hidden" name="order_no"      value="<?= $_POST['order_no'] ?>" />
					<input type="hidden" name="settle_price"  value="12345" />
					<input type="hidden" name="settle_date"   value="<?= date('YmdHis') ?>" />
					<input type="hidden" name="settle_result" value="2" /> <!-- 異常 -->
					<input type="hidden" name="settle_detail" value="11" />
					<input type="hidden" name="settle_method" value="9" /> <!-- VISA -->
					<button type="submit">自動リクエストをSend</button>
				</form>

				<a href="<?= $_POST['failure_url'] ?>" class="button">ユーザが戻る</a>
			</div>

			<div class="column">
                <h2>キャンセル</h2>
				<a href="<?= $_POST['cancel_url'] ?>" class="button">ユーザが戻る</a>
			</div>
		</div>

	</body>
</html>
