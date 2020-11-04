#python3 training.py --cat qg --epoch 20 --predict True --outfile ../output/NN_tuple_qg.root
python3 training.py --cat qg --epoch 20 --predict True --JSS True --outfile ../output/NN_tuple_qg_JSS.root

python3 training.py --cat qq --epoch 20 --predict True --outfile ../output/NN_tuple_qq.root
python3 training.py --cat qq --epoch 20 --predict True --JSS True --outfile ../output/NN_tuple_qq_JSS.root

python3 training.py --cat gg --epoch 20 --predict True --outfile ../output/NN_tuple_gg.root
python3 training.py --cat gg --epoch 20 --predict True --JSS True --outfile ../output/NN_tuple_gg_JSS.root
