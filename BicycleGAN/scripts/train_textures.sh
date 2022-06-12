set -ex
MODEL='bicycle_gan'
# dataset details
CLASS='10color2coloredtexture2'  # facades, day2night, edges2shoes, edges2handbags, maps
NZ=8
NO_FLIP=''
DIRECTION='AtoB'
LOAD_SIZE=512
CROP_SIZE=256
INPUT_NC=3
NITER=300
NITER_DECAY=300
SAVE_EPOCH=50
EPOCH_COUNT=0

# training
GPU_ID=0
DISPLAY_ID=$((GPU_ID*10+1))
CHECKPOINTS_DIR=./checkpoints/${CLASS}/
NAME=${CLASS}_${MODEL}

# command
python ./train.py \
  --display_id -1 \
  --dataroot ./datasets/${CLASS} \
  --name ${NAME} \
  --model ${MODEL} \
  --direction ${DIRECTION} \
  --checkpoints_dir ${CHECKPOINTS_DIR} \
  --load_size ${LOAD_SIZE} \
  --crop_size ${CROP_SIZE} \
  --nz ${NZ} \
  --input_nc ${INPUT_NC} \
  --niter ${NITER} \
  --niter_decay ${NITER_DECAY} \
  --save_epoch_freq ${SAVE_EPOCH} \
  --use_dropout \

# --continue_train \
#--epoch_count ${EPOCH_COUNT} \

