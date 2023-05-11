import express from 'express';
import { getPostById, getTop } from '../controllers/posts.js';
import { getTags,getUserDisplayNames,getUserDisplayNames2, getUserInfo,login,logout,refreshToken} from '../controllers/users.js';
import { verifyToken,Testing } from '../middleware/verifyToken.js';

const router = express.Router();

router.get('/tag/:id',getTags);
router.get('/names', getUserDisplayNames2);
router.get('/names/:id',getUserDisplayNames);
router.get('/info/:id',getUserInfo);

router.post('/getPostsById',getPostById);
router.get('/getTop',getTop);

router.post('/login',login);
router.delete('/logout/:id',logout);

router.get('/refreshToken',refreshToken);
router.get('/verifyToken',verifyToken,Testing);

export default router;