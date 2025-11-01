# How To

1. create the executable by running `make` or `make all` in the terminal
   * resulting executable is called `msg_test`
2. run the executable in the terminal `./msg_test`

# How to know the code works

`msg_test` will print what tests are being run and if there are any failed tests it will print "---- FAILED TEST" along with more details on the failed test. At the end if it prints "TOTAL TESTS FAILED = 0" that means all tests were passed.

inside the `message_store` directory there should be the following

* `id_counter.txt` which is reset to 0 when `msg_test` runs and then increments everytime we use the `create_msg` function
* `messages.csv` which should have the end state of the message store after running the tests
* `history` subdirectory that holds previous versions of the message store during testing
  * `1_after_happy_path.csv` should have the 5 message objects that were stored in lines 388-392 of `test_message.c`
  * `2_after_null_object.csv` should look the same after trying to store a NULL object in line 396 of `test_message.c`
  * `3_after_duplicate_object.csv` should look the same after trying to store an existing message in line 400 of `test_message.c`
  * `4_after_update_object.csv` should look the same except for the second message that has been edited and then stored in line 406 of `test_message.c`
